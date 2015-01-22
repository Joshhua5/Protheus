/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 9:01:2015: Waring J.
*************************************************************************/
#pragma once

const unsigned DEFAULT_PARALLEL_THREAD_COUNT = 4;

#include <thread>    
#include <functional>
#include <condition_variable>
#include <mutex>  
#include "ObjectPool.h"
#include "Queue.h" 
#include "Future.h"

namespace Pro {
	namespace Util {


		/*! Parallel class used to execute a function asynchronously,
			(Not implemented)
			Process jobs have higher priority that queue jobs and will take control of all workers until the process is finished
			and then the queue jobs will start again

			TODO:
			Add wait events to control the order of job execution
			*/
		class Parallel {

			struct BatchPack {
				std::atomic<bool> being_processed;
				Future* finished;
				std::function<void(void)> function;

				inline BatchPack& operator=(const BatchPack& rhs) {
					finished = rhs.finished;						// Function doesn't get copied
					being_processed = rhs.being_processed.load();	// as it is replaced in each case inside the Parallel Class 
					return *this;									// function = rhs.function;
				}
				BatchPack(Future* finished) {
					this->finished = finished;
					being_processed = false;
				}
			};


			std::mutex m;
			std::condition_variable cv;
			ObjectPool<BatchPack> obj_pool;
			Queue<BatchPack*> work;
			std::atomic<bool> pool_running;
			std::once_flag initialized;
			Future default_result;
			std::thread* threads;
			unsigned thread_count;

			void workerThread() {
				BatchPack* item;
				std::unique_lock<std::mutex> lk(m);
				while (pool_running.load())
					// Get a work item when it's ready 
					if (work.empty())
						cv.wait_for(lk, std::chrono::milliseconds(100));
					else {
						item = work.pop();
						if (item == nullptr || item->being_processed == true)
							continue;
						item->being_processed = true;
						item->function();
						// Cleanup work item
						item->finished->thread_finished();
						delete item;
					}
			}


		public:
			Parallel(unsigned count = DEFAULT_PARALLEL_THREAD_COUNT) {
				pool_running.store(true);
				work.resize(500);
				threads = new std::thread[count];
				for (unsigned x = 0; x < count; ++x)
					threads[x] = std::thread(&Parallel::workerThread, this);
				thread_count = count;
			}

			~Parallel() {
				pool_running.store(false);
				cv.notify_all();
				for (unsigned x = 0; x < thread_count; ++x)
					if (threads[x].joinable())
						threads[x].join();
				delete[] threads;
			}

			bool isQueueEmpty() {
				return work.empty();
			}

			template<typename T, typename... Args>
			void batch(T* func, Future* finished, Args... arguments) {
				if (finished == nullptr)
					finished = &default_result;

				finished->worker_count = 1;
				finished->finished_count = 0;
				auto pack = new BatchPack(finished);
				pack->function = [=]() { std::bind(func, arguments...)(); };
				work.push(pack);
				cv.notify_one();
			}

			template<typename T, typename... Args>
			void batch(Future* finished, void* data, Args... arguments) {
				if (finished == nullptr)
					finished = &default_result;
				pack->finished = finished;

				finished->worker_count = 1;
				finished->finished_count = 0;

				auto pack = new BatchPack(finished);
				pack->function = [=]() { std::bind(func, data, arguments...)(); };
				work.push(pack);

				cv.notify_one(); 
			}

			/*!	Process objects and calls object methods
				Accepts a function pointer of a member function
				If future is nullptr then no finished event is set.
			*/
			template<typename T, typename F, typename... Args>
			void process(T* object, F func, unsigned size, unsigned offset, Future* finished, Args... arguments) {
				if (finished == nullptr)
					finished = &default_result;

				finished->worker_count = thread_count;
				finished->finished_count = 0;

				// if less than thread_count
				if (size < thread_count) {
					finished->worker_count = size;
					for (unsigned x = 0; x < size; ++x) {
						auto pack = new BatchPack(finished);
						pack->function = [=]() { std::bind(func, &object[x], arguments...)(); };
						work.push(pack);
					}
					cv.notify_all();
					return;
				}

				// if not divisible perfectly by thread_count
				const unsigned displacement = size % thread_count;
				// Batch work which is thread divible
				size -= displacement;

				// Set worker count before batching, otherwise future could be set as true if the last condition
				// isn't executed before the threads finished.
				if(displacement != 0)
					++finished->worker_count;

				for (unsigned work_segment = 0; work_segment < thread_count; ++work_segment) {
					const unsigned segment_size = size / thread_count;
					auto pack = new BatchPack(finished);
					pack->function = [=]() {
						const unsigned _offset = work_segment * segment_size;
						const unsigned end = _offset + segment_size;
						for (unsigned current = _offset; current < end; ++current)
							std::bind(func, &object[current], arguments...)();
					};
					work.push(pack);
				}
				// Batch the remaining objects which were removed to allow for correct division
				if (displacement != 0) {
					auto pack = new BatchPack(finished);
					pack->function = [=]() {
						for (unsigned x = size; x < displacement; ++x)
							std::bind(func, &object[x], arguments...)();
					};
					work.push(pack);
				}
				cv.notify_all();
			}

			/*! Process for data without member functions,
				Accepts a function of void()(void*, size_t size, size_t offset, Args... arguments)
				If future is nullptr then no finished event is set.
			*/
			template<typename F, typename... Args>
			void process(F func, void* data, unsigned size, unsigned offset, Future* finished, Args... arguments) {

			}
		};

		static Parallel parallel;
	}
}