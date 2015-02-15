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


			std::mutex work_lock_;
			std::condition_variable new_work_; 
			Queue<BatchPack*> work_;
			std::atomic<bool> pool_running_;
			std::once_flag initialized_;
			Future default_result_;
			std::thread* workers_;
			unsigned thread_count;

			void workerThread() {
				BatchPack* item;
				std::unique_lock<std::mutex> lk(work_lock_);
				while (pool_running_.load())
					// Get a work item when it's ready 
					if (work_.Empty())
						new_work_.wait_for(lk, std::chrono::milliseconds(100));
					else {
						item = work_.TopPop();
						if (item == nullptr || item->being_processed == true)
							continue;
						item->being_processed = true;
						item->function();
						// Cleanup work item
						item->finished->ThreadFinished();
						delete item;
					}
			}
			  
		public:
			Parallel(unsigned pool_size) {
				pool_running_.store(true);
				work_.Resize(500);
				workers_ = reinterpret_cast<std::thread*>(operator new(sizeof(std::thread) * pool_size)); 
				for (unsigned x = 0; x < pool_size; ++x) 
					new(&workers_[x]) std::thread(&Parallel::workerThread, this); 
				thread_count = pool_size;
			}

			~Parallel() {
				pool_running_.store(false);
				new_work_.notify_all();
				for (unsigned x = 0; x < thread_count; ++x) {
					if (workers_[x].joinable())
						workers_[x].join();
					workers_[x].~thread();
				}
				operator delete(workers_);
			}

			bool IsQueueEmpty() {
				return work_.Empty();
			}

			template<typename T, typename... Args>
			void Batch(T* func, Future* finished, Args... arguments) {
				if (finished == nullptr)
					finished = &default_result_;

				finished->worker_count_ = 1;
				finished->finished_count_ = 0;
				auto pack = new BatchPack(finished);
				pack->function = [=]() { std::bind(func, arguments...)(); };
				work_.Push(pack);
				new_work_.notify_one();
			}

			template<typename T, typename... Args>
			void Batch(T* func, Future* finished, void* data, Args... arguments) {
				if (finished == nullptr)
					finished = &default_result_;
				pack->finished = finished;

				finished->worker_count_ = 1;
				finished->finished_count_ = 0;

				auto pack = new BatchPack(finished);
				pack->function = [=]() { std::bind(func, data, arguments...)(); };
				work_.Push(pack);

				new_work_.notify_one(); 
			}

			/*!	Process objects and calls object methods
				Accepts a function pointer of a member function
				If future is nullptr then no finished event is set.
			*/
			template<typename T, typename F, typename... Args>
			void Process(T* object, F func, unsigned size, unsigned offset, Future* finished, Args... arguments) {
				if (finished == nullptr)
					finished = &default_result_;

				finished->worker_count_ = thread_count + 1;
				finished->finished_count_ = 0;

				// if less than thread_count
				if (size < thread_count) {
					finished->worker_count_ = size;
					for (unsigned x = 0; x < size; ++x) {
						auto pack = new BatchPack(finished);
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

				// Set worker count before batching, otherwise future could be set as true if the last condition
				// isn't executed before the threads finished. 

				for (unsigned work_segment = 0; work_segment < thread_count; ++work_segment) {
					// Worker count or stored assuming one thread will be used to fix alignment, correc the value here.
					// This reduces the amount of conditions by 1
					const unsigned segment_size = size / thread_count;
					auto pack = new BatchPack(finished);
					pack->function = [=]() {
						const unsigned _offset = work_segment * segment_size;
						const unsigned end = _offset + segment_size;
						for (unsigned current = _offset; current < end; ++current)
							std::bind(func, &object[current], arguments...)();
					};
					work_.Push(pack);
				}
				// Batch the remaining objects which were removed to allow for correct division
				if (displacement != 0) {
					auto pack = new BatchPack(finished);
					pack->function = [=]() {
						for (unsigned x = size; x < size + displacement; ++x)
							std::bind(func, &object[x], arguments...)();
					};
					work_.Push(pack);
				}else
					--finished->worker_count_;
				new_work_.notify_all();
			}

			/*! Process for data without member functions,
				Accepts a function of void()(void*, size_t size, size_t offset, Args... arguments)
				If future is nullptr then no finished event is set.
			*/
			template<typename F, typename... Args>
			void Process(F func, void* data, unsigned size, unsigned offset, Future* finished, Args... arguments) {

			}
		}; 
	}
}