/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 9:01:2015: Waring J.
*************************************************************************/
#pragma once

#include <thread> 
#include "Timer.h"
#include <functional>
#include <condition_variable>
#include <mutex> 
#include "mcmp_queue.h"
#include "ObjectPool.h"
#include "Queue.h"
#include "Log.h"
#include "Sync.h"

namespace Pro {
	namespace Util {


		/*! Parallel class used to execute a function asynchronously,
			(Not implemented)
			Process jobs have higher priority that queue jobs and will take control of all workers until the process is finished
			and then the queue jobs will start again

			*/
		class Parallel {
			struct BatchPack {
				Sync* finished;
				Sync* start;
				std::function<void(void)> function;
				//! Time that the job is scheduled to start
				std::chrono::milliseconds start_time;

				inline BatchPack& operator=(const BatchPack& rhs) {
					finished = rhs.finished;						// Function doesn't get copied
																	// as it is replaced in each case inside the Parallel Class
					return *this;									// function = rhs.function;
				}
				BatchPack(Sync* finished, Sync* start) {
					this->finished = finished;
					this->start = start;
					start_time = std::chrono::milliseconds(0);
				}
			};

			 
			std::condition_variable new_work_;
			ObjectPool<BatchPack> pack_pool_;
			Util::mcmp_queue<BatchPack*> work_;
			std::atomic<bool> pool_running_;
			std::once_flag initialized_;
			std::atomic<unsigned> thread_count;

			void workerThread() {
				BatchPack* item;
				while (pool_running_)
					// Get a work item when it's ready
					if (work_.Pop(item)) {
						//! A pointer of 1 is a signal to terminate this thread
						if (item == (BatchPack*)1) {
							thread_count.fetch_sub(1); 
							break;
						}

						if (item->start_time.count() != 0) {
							// Check that the item is ready
							if (GetGlobalTimer().GetTime<std::chrono::milliseconds>() >= item->start_time.count()) {
								work_.Push(item); 
								continue;
							}
						}

						if (item->start != nullptr) {
							// Check that the item is ready
							if (!item->start->IsFinished()) { 
								work_.Push(item); 
								continue;
							}
						}

						if (item->function == nullptr) {
							global_log.Report<LogCode::FAULT>("Nullptr function passed to Parallel Class", __FUNCTION__, __LINE__);
							pack_pool_.Return(item);
							continue;
						}
						item->function();
						// Cleanup work item
						if (item->finished != nullptr)
							item->finished->ThreadFinished();
						pack_pool_.Return(item);
					}
					else {
						// Check if an thread needs to terminate
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
					}
			}

		public:
			Parallel(unsigned pool_worker_size) : work_(2048, 0) {
				pool_running_ = true;
                thread_count = 0;
                AddWorker(pool_worker_size);
			}

			~Parallel() {
				// Set flag for threads to close
				pool_running_ = false;
				new_work_.notify_all();
			}
  
            //! Add a function to the Parallel work queue and passes @data to the function
			//! Batch will execute once @start is finished
            //! If the batch doesn't have any dependencies, than pass nullptr into start
			template<typename T, typename... Args>
			void Batch(T func, Sync* finished, Sync* start, long long delay_ms, Args... arguments) {
				if (finished != nullptr)
					finished->AddWorker(1);
				auto pack = pack_pool_.Get(finished, start);
                pack->start_time = (delay_ms > 0) ?
                    std::chrono::milliseconds(GetGlobalTimer().GetTime<std::chrono::milliseconds>() + delay_ms) :
                    std::chrono::milliseconds(0);
                
				pack->function = [=]() { std::bind(func, arguments...)(); }; 
				work_.Push(pack); 

				new_work_.notify_one();
			}

			template<typename O, typename T, typename... Args>
			void MemberBatch(T func, Sync* finished, O* this_ptr, Args... arguments) {
				if (finished != nullptr)
					finished->AddWorker(1);

				auto pack = pack_pool_.Get(finished, nullptr);
				pack->function = [=]() { std::bind(func, this_ptr, arguments...)(); }; 
				work_.Push(pack);  
				new_work_.notify_one();
			}

			/*!	Process objects and calls object methods
				Accepts a function pointer of a member function
				If future is nullptr then no finished event is set.
			*/
			template<typename T, typename F, typename... Args>
			void Process(T* object, F func, unsigned size, unsigned offset, Sync* finished, Args... arguments) {
				// Pay for one atomic load on thread_count
				unsigned thread_count = this->thread_count.load();

				// if less than thread_count
				if (size < thread_count) {
					if (finished != nullptr)
						finished->AddWorker(size);
					for (unsigned x = 0; x < size; ++x) {
						auto pack = pack_pool_.Get(finished, nullptr);
						pack->function = [=]() { std::bind(func, &object[x], arguments...)(); };
						work_.Push(pack);
					}
					new_work_.notify_all();
					return;
				}


				// if not divisible perfectly by thread_count
				const unsigned displacement = size % thread_count;
				// Batch work which is thread divible
				size -= displacement;

				if (finished != nullptr)
					finished->AddWorker((displacement != 0) ? thread_count + 1 : thread_count);

				// Set worker count before batching, otherwise future could be set as true if the last condition
				// isn't executed before the threads finished.
 
				for (unsigned work_segment = 0; work_segment < thread_count; ++work_segment) {
					// Worker count or stored assuming one thread will be used to fix alignment, correct the value here.
					// This reduces the amount of conditions by 1
					const unsigned segment_size = size / thread_count;
					const unsigned _offset = work_segment * segment_size;
					const unsigned end = _offset + segment_size;
					auto pack = pack_pool_.Get(finished, nullptr);
					pack->function = [=]() {
						for (unsigned current = _offset; current < end; ++current)
							std::bind(func, &object[current], arguments...)();
					};
					work_.Push(pack);
				}
				// Batch the remaining objects which were removed to allow for correct division
				if (displacement != 0) {
					auto pack = pack_pool_.Get(finished, nullptr);
					pack->function = [=]() {
						for (unsigned x = size; x < size + displacement; ++x)
							std::bind(func, &object[x], arguments...)();
					};
					work_.Push(pack);
				}
				new_work_.notify_all();
			} 

			void AddWorker(const unsigned count) {
				for (unsigned x = 0; x < count; ++x)
					std::thread(&Parallel::workerThread, this).detach();
				thread_count += count;
			}

			unsigned WorkerCount() const {
				return thread_count.load();
			}

			void KillWorker(unsigned count) {
				if (count > thread_count)
					count = thread_count;
				for (unsigned x = 0; x < count; ++x) { 
					if (!work_.Push((BatchPack*)1))
						global_log.Report<LogCode::FAULT>("Unable to terminate a worker thread, is the work queue full?", __FUNCTION__, __LINE__);
				}
			}
		};

        //! Returns a general purpose worker pool
        static inline Parallel& GetGlobalWorkPool(){
            static Parallel global(std::thread::hardware_concurrency());
            return global;
        } 
	}
}
