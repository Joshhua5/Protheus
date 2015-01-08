#pragma once

#include <thread>   
#include <functional>
#include <condition_variable>
#include <mutex>  
#include "ObjectPool.h"
#include "LinkedList.h" 
#include "Future.h"

namespace Pro {
	namespace Util {

		namespace Parallel_data {

			struct BatchPack {
				std::atomic<bool> being_processed;
				Future* finished;
				std::function<void(void)> function;
				BatchPack(){}
				~BatchPack() {}

				inline BatchPack& operator=(const BatchPack& rhs){
					finished = rhs.finished;
					// Function doesn't get copied
					// as it is replaced in each case inside the Parallel Class
					//function = rhs.function;
					being_processed = rhs.being_processed.load();
					return *this;
				}
			};

			static std::mutex m;
			static std::condition_variable cv;
			static std::atomic<bool> pool_running;
			static ObjectPool<BatchPack> obj_pool;
			static LinkedList<BatchPack> work;
			static std::once_flag initialized;
			static Future default_result;
			static unsigned thread_count;
		}

		using namespace Parallel_data;

		/*! Parallel class used to execute a function asynchronously, functions must have a void return and a void* argument
			Process jobs have higher priority that queue jobs and will take control of all workers until the process is finished
			and then the queue jobs will start again
			*/
		class Parallel {

			static void workerThread() {
				BatchPack* item;
				std::unique_lock<std::mutex> lk(m);
				while (pool_running.load()) {
					// Get a work item when it's ready 
					if (work.empty()){ 
						cv.wait_for(lk, std::chrono::milliseconds(500));
					}else{
						item = work.pop_back();
						if (item == nullptr)
							continue;

						item->function();

						// Cleanup work item
						item->finished->thread_finished(); 
						delete item;
					} 
				}
			}

		public:
			Parallel(unsigned count = 2) {
				std::call_once(initialized,
					[](unsigned count) {
					pool_running.store(true);
					for (unsigned x = 0; x < count; ++x)
						std::thread(&workerThread).detach();
				}, count);
				thread_count = count;
			}

			~Parallel() { 
				pool_running.store(false);
				cv.notify_all(); 
			}

			static bool isQueueEmpty(){
				return work.empty();
			}
  
			template<typename T, typename... Args>
			static void batch(T* func, Future* finished, Args... arguments){
				if (finished == nullptr)
					finished = &default_result;

				BatchPack* pack = new BatchPack();
				
				pack->finished = finished;
				pack->finished->worker_count = 1;
				pack->finished->finished_count = 0; 

				pack->function = [=]() { std::function<T> f(func); f(arguments...); };

				work.push_front(pack);
				cv.notify_one();
			}

			template<typename T, typename F, typename... Args>
			static void process(T* object, F func, unsigned size, unsigned offset, Future* finished, Args... arguments) {
				if (finished == nullptr)
					finished = &default_result;

				BatchPack pack; 
				BatchPack* packCopy;

				pack.finished = finished;
				pack.finished->finished_count = 0;

				// if less than thread_count
				if (size < thread_count) {
					pack.finished->worker_count = size;
					for (unsigned x = 0; x < size; ++x) {
						BatchPack* packCopy = new BatchPack();
						*packCopy = pack; 

						packCopy->function = [=]() {
							auto f = std::bind(func, object[x], arguments...); 
							f();
						};
						work.push_front(packCopy);
						cv.notify_all();
					}
					return;
				}

				pack.finished->worker_count = thread_count;

				// if not divisible perfectly by thread_count
				if (size % thread_count != 0) {
					unsigned displacement = size % thread_count;
					// Batch work which is thread divible
					size -= displacement;
					pack.finished->worker_count = thread_count + 1;

					for (unsigned work_segment = 0; work_segment < thread_count; ++work_segment) {
						packCopy = new BatchPack();
						*packCopy = pack;
						packCopy->function = [=]() {
							for (unsigned x = work_segment * (size / thread_count); x < size / thread_count; ++x) { 
								auto f = std::bind(func, object[x], arguments...);
								f();
							}};
						work.push_front(packCopy);
						cv.notify_all();

					}
					// Batch the remaining objects which were removed to allow for correct division
					packCopy = new BatchPack();
					*packCopy = pack;
					packCopy->function = [=]() {
						for (unsigned x = size; x < displacement; ++x) {
							auto f = std::bind(func, object[x], arguments...);
							f();
						}
					};
					work.push_front(packCopy);
					cv.notify_all();
					return;
				}

				pack.finished->worker_count = thread_count;
				for (unsigned work_segment = 0; work_segment < thread_count; ++work_segment) {
					packCopy = new BatchPack();
					*packCopy = pack;
					packCopy->function = [=]() {
						for (unsigned x = work_segment * (size / thread_count); x < size / thread_count; ++x) {
							auto f = std::bind(func, object[x], arguments...);
							f();
						}
					};
					work.push_front(packCopy);
					cv.notify_all();
				}
			}
	

		};

		static Parallel parallel;
	}
}