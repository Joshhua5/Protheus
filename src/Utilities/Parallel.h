#pragma once

#include <thread>  
#include <queue> 
#include <atomic>
#include <mutex> 
#include <condition_variable>
#include <unordered_map>
#include "ArrayList.h"
#include "LinkedList.h"

namespace Pro {
	namespace Util {

		typedef void(*thread_func)(void*);

		namespace Parallel_data {
			struct BatchPack {
				std::atomic<bool> being_processed;
				thread_func func;
				void* argument;
				volatile bool* finished;

				// Used for ArrayPack
				void(*array_proc)(BatchPack*);
				void* object;
				unsigned offset, size;
				~BatchPack() {}
			};

			static std::atomic<bool> pool_running;
			static LinkedList<BatchPack> work;
			static std::once_flag initialized;
			static bool default_result = false;
			static unsigned thread_count;
		}
		using namespace Parallel_data;

		/*! Parallel class used to execute a function asynchronously, functions must have a void return and a void* argument
			Process jobs have higher priority that queue jobs and will take control of all workers until the process is finished
			and then the queue jobs will start again
			*/
		class Parallel {

			template<typename T>
			static void array_processor(BatchPack* item) {
				// Array work item 
				for (unsigned x = 0; x < item->size ; x++) {
					union {
						thread_func i_func;
						void(__thiscall T::* o_func)(void*);
					};
					i_func = item->func;
					auto obj = &static_cast<T*>(item->object)[item->offset + x];
					(obj->*o_func)(item->argument);
				}
			}  
			static void workerThread() {
				while (pool_running.load()) {
					// Get a work item when it's ready 
					if (work.empty())
						std::this_thread::yield();
					else {
						BatchPack* item = work.remove(0);
						if (item == nullptr)
							continue;

						if (item->object == nullptr)
							item->func(item->argument);
						else
							item->array_proc(item);

						// Cleanup work item
						*(item->finished) = true;
						delete item;
					}
				}
			}

		public:
			Parallel(unsigned count = 8) {
				std::call_once(initialized,
					[](int count) {
					pool_running.store(true);
					for (unsigned x = 0; x < count; ++x)
						std::thread(&workerThread).detach();
				}, count); 
				thread_count = count;
			}

			~Parallel() {
				/*	while (!work.empty() && !array_pool.empty())
					*/	std::this_thread::sleep_for(std::chrono::microseconds(10));
			pool_running.store(false);
			}

			static void batch(thread_func func, volatile bool* finished = nullptr, void* argument = nullptr) {
				if (finished != nullptr)
					*finished = false;
				else
					finished = &default_result;
				BatchPack* pack = new BatchPack();
				pack->func = func;
				pack->argument = argument;
				pack->finished = finished;
				pack->object = nullptr;
				work.append(pack);
				//master_wait.notify_one();
			}
			 
			template<class T>
			static void process(T* object, void(T::*func)(void*), unsigned size, unsigned offset, volatile bool* finished = nullptr, void* argument = nullptr) {
				if (finished != nullptr)
					*finished = false;
				else
					finished = &default_result;

				BatchPack pack;

				union {
					thread_func i_func;
					void(__thiscall T::* o_func)(void*);
				};

				o_func = func;
				pack.func = i_func;
				pack.argument = argument;
				pack.finished = finished;
				pack.object = object;
				pack.array_proc = &array_processor<T>;

				// if less than thread_count
				if (size < thread_count) {
					for (unsigned x = 0; x < size; ++x) {
						BatchPack* seg = new BatchPack();
						memcpy(seg, &pack, sizeof(BatchPack));
						seg->size = 1;
						seg->offset = x;
						work.append(seg);
					}
					return;
				} 
				 
				// if not divisible perfectly by thread_count
				if (size % thread_count != 0) {
					unsigned displacement = size % thread_count;
					// Batch work which is thread divible
					size -= displacement;
					pack.size = size / thread_count;
					BatchPack* seg;

					for (unsigned work_segment = 0; work_segment < thread_count; ++work_segment) {
						 seg = new BatchPack();
						memcpy(seg, &pack, sizeof(BatchPack));
						seg->offset = work_segment * (size / thread_count);
						work.append(seg);
					}
					// Batch the remaining objects which were removed to allow for correct division
					seg = new BatchPack();
					memcpy(seg, &pack, sizeof(BatchPack));
					seg->offset = size;
					seg->size = displacement;
					work.append(seg); 
					return;
				}

				pack.size = size / thread_count;
				for (unsigned work_segment = 0; work_segment < thread_count; ++work_segment) {
					BatchPack* seg = new BatchPack();
					memcpy(seg, &pack, sizeof(BatchPack));
					seg->offset = work_segment * (size / thread_count);
					work.append(seg);
				}
			}
		};

		static Parallel parallel;
	}
}