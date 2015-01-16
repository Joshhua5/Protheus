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
						if (item == nullptr)
							continue;
						item->function();
						// Cleanup work item
						item->finished->thread_finished();
						delete item;
					} 
			}

		public:
			Parallel(unsigned count = DEFAULT_PARALLEL_THREAD_COUNT) {
				pool_running.store(true);
				threads = new std::thread[count];
				for (unsigned x = 0; x < count; ++x)
					threads[x] = std::thread(&Parallel::workerThread, this);
				thread_count = count;
			}
			 
			~Parallel()  { 
				pool_running.store(false);
				cv.notify_all();
				for (unsigned x = 0; x < thread_count; ++x)
					if(threads[x].joinable())
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

				BatchPack* pack = new BatchPack();

				pack->finished = finished;
				pack->finished->worker_count = 1;
				pack->finished->finished_count = 0;

				pack->function = [=]() { std::bind(func, arguments...)(); };

				work.push(pack);
				cv.notify_one();
			}

			template<typename T, typename F, typename... Args>
			void process(T* object, F func, unsigned size, unsigned offset, Future* finished, Args... arguments) {
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
						packCopy = new BatchPack();
						*packCopy = pack;

						packCopy->function = [=]() {
							std::bind(func, &object[x], arguments...)();
						};
						work.push(packCopy);
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
						unsigned segment_size = size / thread_count;
						packCopy->function = [=]() {
							unsigned _offset = work_segment * segment_size;
							unsigned end = _offset + segment_size;
							for (unsigned current = _offset; current < end; ++current)
								std::bind(func, &object[current], arguments...)();
						};
						work.push(packCopy);
						cv.notify_all();

					}
					// Batch the remaining objects which were removed to allow for correct division
					packCopy = new BatchPack();
					*packCopy = pack;
					packCopy->function = [=]() {
						for (unsigned x = size; x < displacement; ++x)
							std::bind(func, &object[x], arguments...)();
					};
					work.push(packCopy);
					cv.notify_all();
					return;
				}

				pack.finished->worker_count = thread_count;
				for (unsigned work_segment = 0; work_segment < thread_count; ++work_segment) {
					packCopy = new BatchPack();
					*packCopy = pack;
					unsigned segment_size = size / thread_count;
					packCopy->function = [=]() {
						unsigned _offset = work_segment * segment_size;
						unsigned end = _offset + segment_size;
						for (unsigned current = _offset; current < end; ++current)
							std::bind(func, &object[current], arguments...)();
					};
					work.push(packCopy);
					cv.notify_all();
				}
			}


		};

		static Parallel parallel;
	}
}