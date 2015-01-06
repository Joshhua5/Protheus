#pragma once

#include <thread>   
#include <functional>
#include <mutex>  
#include "LinkedList.h" 
#include "Future.h"

namespace Pro {
	namespace Util {

		namespace Parallel_data {

			struct BatchPack {
				std::atomic<bool> being_processed;  
				Future* finished; 
				std::function<void(void)> function;   
				~BatchPack() {}
			};

			static std::atomic<bool> pool_running;
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
				while (pool_running.load()) {
					// Get a work item when it's ready 
					if (work.empty())
						std::this_thread::yield();
					else {
						BatchPack* item = work.remove(0);
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
			}

			template<typename T, typename... Args>
			static void batch(std::function<T> func, Future* finished, Args... arguments) {
				 
				if (finished == nullptr)
					finished = &default_result;

				BatchPack* pack = new BatchPack();  
				pack->finished = finished;
				pack->finished->worker_count = 1;
				pack->finished->finished_count = 0; 
				   
				pack->function = [=]() {func(arguments...);};
				 
				work.append(pack);
			}  
			 
			template<typename T, typename F, typename... Args>
			static void process(T* object, std::function<F> func, unsigned size, unsigned offset, Future* finished , Args... arguments) {
				if (finished == nullptr)
					finished = &default_result;

				BatchPack pack;
				    
				pack.finished = finished; 
				pack.finished->finished_count = 0; 

				// if less than thread_count
				if (size < thread_count) { 
					pack.finished->worker_count = size;
					for (unsigned x = 0; x < size; ++x) {
						BatchPack* seg = new BatchPack();
						memcpy(seg, &pack, sizeof(BatchPack));

						seg->function = [=]() { func(object[x], arguments...); };
						work.append(seg);
					}
					return;
				}  

				pack.finished->worker_count = thread_count;

				// if not divisible perfectly by thread_count
				if (size % thread_count != 0) {
					unsigned displacement = size % thread_count;
					// Batch work which is thread divible
					size -= displacement; 
					BatchPack* seg;
					pack.finished->worker_count = thread_count + 1;

					for (unsigned work_segment = 0; work_segment < thread_count; ++work_segment) {
						seg = new BatchPack();
						memcpy(seg, &pack, sizeof(BatchPack));
						seg->function = [=]() {
							for (unsigned x = work_segment * (size / thread_count); x < size/thread_count; ++x) { 
							func(object[x], arguments...); }};
						work.append(seg);
						
					}
					// Batch the remaining objects which were removed to allow for correct division
					seg = new BatchPack();
					memcpy(seg, &pack, sizeof(BatchPack));
					seg->function = [=]() {
						for (unsigned x = size; x < displacement; ++x) { func(object[x], arguments...); }}; 
					work.append(seg); 
					return;
				}
				 
				pack.finished->worker_count = thread_count;
				for (unsigned work_segment = 0; work_segment < thread_count; ++work_segment) {
					BatchPack* seg = new BatchPack();
					memcpy(seg, &pack, sizeof(BatchPack)); 
					seg->function = [=]() {
						for (unsigned x = work_segment * (size / thread_count); x < size / thread_count; ++x) { func(object[x], arguments...); }};
					work.append(seg);
				}
			}
		};

		static Parallel parallel;
	}
}