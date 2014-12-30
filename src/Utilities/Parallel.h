#pragma once

#include <thread>  
#include <queue> 
#include <unordered_map>
#include <vector>

namespace Pro {
	namespace Util {
		typedef void(*thread_func)(void*);
		/*! Package of static data for the Parallel class */
		namespace Parallel_data {
			struct BatchPack {
				thread_func func;
				void* argument;
				bool* finished;

				// Used for ArrayPack
				void(*array_proc)(BatchPack*);
				void* object;
				unsigned offset, size;
				~BatchPack() {}
			};

			static std::queue<std::pair<BatchPack*, size_t>> array_pool;
			static std::queue<BatchPack*> batch_pool;
			static std::vector<std::pair<std::thread, BatchPack**>> workers;
			// used to allow pass by reference without destroying the object
			static std::vector<std::thread> typed_workers;
			static std::thread master;
			static bool pool_running;
			/*! Only used to give a valid pointer to a worker*/
			static bool default_result = false;
		}

		using namespace Parallel_data;
		/*! Parallel class used to execute a function asynchronously, functions must have a void return and a void* argument
			Process jobs have higher priority that queue jobs and will take control of all workers until the process is finished
			and then the queue jobs will start again
		*/
		class Parallel {
			static std::unordered_map <size_t, std::vector < std::pair<std::thread&, BatchPack**> >> &type_workers() {
				static std::unordered_map<size_t, std::vector<std::pair<std::thread&, BatchPack**>>> typer_worker_vector(1);
				return typer_worker_vector;
			}

			template<typename T>
			static void array_processor(BatchPack* item) {
				// Array work item 
				for (unsigned x = item->offset; x < item->size; x++) {
					union {
						thread_func i_func;
						void(__thiscall T::* o_func)(void*);
					};
					i_func = item->func;
					auto obj = &static_cast<T*>(item->object)[x];
					(obj->* o_func)(item->argument);
				}
			}

			static void workerThread(BatchPack** work) {
				while (pool_running) {
					// Get a work item when it's ready 
					if (*work == nullptr)
						std::this_thread::sleep_for(std::chrono::microseconds(10));
					else if ((*work)->object == nullptr) {
						// Batch work item
						BatchPack* item = *work;
						item->func(item->argument);

						// Prepare for more work  
						*work = nullptr;
						*(item->finished) = true;
						delete item;
					}
					else { 
						BatchPack* item = *work; 
						item->array_proc(item);

						// Prepare for more work  
						*work = nullptr;
						*(item->finished) = true;
						delete item;
					}
				}
			}

			static void masterThread() {
				while (pool_running) {
					// Process arrays
					if (!array_pool.empty()) {
						unsigned worker_size = workers.size();
						std::pair<BatchPack*, size_t> job = std::move(array_pool.front());
						array_pool.pop();
						auto t_workers = type_workers();

						// Check if threads already exist
						if (t_workers.find(job.second) == t_workers.end()) { 
							std::vector<std::pair<std::thread&, BatchPack**>> temp;
							auto inserted = t_workers.insert(std::make_pair( job.second, std::move(temp))).first;
							// Create threads for this data type
							for (unsigned x = 0; x < worker_size; ++x) {
								BatchPack** y = new BatchPack*;
								*y = nullptr;
								typed_workers.push_back(std::thread(&workerThread, y));
								inserted->second.push_back({ typed_workers.back(),  y });
							}
						}

						auto array_workers = t_workers.at(job.second);
						bool* finished = new bool[worker_size];
						for (unsigned segment = 0; segment < worker_size; ++segment) {
							// Create a work item for the job
							BatchPack* item = new BatchPack; 
							memcpy(item,job.first, sizeof(BatchPack));
							item->size = job.first->size / worker_size;
							item->offset = segment * (job.first->size / worker_size);
							item->finished = finished + segment; 
							// Find a thread to pass the work item 
							for (unsigned x = 0; x < worker_size; ++x)
								if (*(array_workers[x].second) == nullptr) {
									*(array_workers[x].second) = item;
									break;
								}
						}

						// Wait for all threads to finish
						for (unsigned x = 0; x < worker_size; ++x)
							if (finished[x] == false) {
								x = 0;
								std::this_thread::sleep_for(std::chrono::microseconds(3));
							}
						*job.first->finished = true;

						delete[] finished;
					}

					// Process pool
					if (!batch_pool.empty()) {
						unsigned worker_size = workers.size();
						for (unsigned x = 0; x < worker_size && !batch_pool.empty(); ++x) {
							if (*workers[x].second == nullptr) {
								*workers[x].second = batch_pool.front();
								batch_pool.pop();
							}
						}
					}
					std::this_thread::sleep_for(std::chrono::microseconds(10));
				}
			}

		public:
			Parallel(unsigned count = 4) {
				pool_running = true;
				master = std::thread(masterThread);
				workers;
				for (unsigned x = 0; x < count; ++x) {
					BatchPack** y = new BatchPack*;
					*y = nullptr;
					workers.push_back({ std::thread(workerThread, y),  y });
				}
			}

			~Parallel() {
				while (!batch_pool.empty() && !array_pool.empty())
					std::this_thread::sleep_for(std::chrono::microseconds(10));
				pool_running = false;
				master.detach();
				for (auto& threads : workers)
					threads.first.detach(); 
				for (auto& threads : typed_workers)
					threads.detach();
			}

			inline static void batch(thread_func func, void* argument = nullptr, bool* finished = nullptr) {
				if (finished != nullptr)
					*finished = false;
				else
					finished = &default_result;
				BatchPack* pack = new BatchPack();
				pack->func = func;
				pack->argument = argument;
				pack->finished = finished;
				pack->object = nullptr;
				batch_pool.push(pack);
			}

			template<class T>
			inline static void process(T* object, void(T::*func)(void*), unsigned size, unsigned offset, void* argument = nullptr, bool* finished = nullptr) {
				if (finished != nullptr)
					*finished = false;
				else
					finished = &default_result;

				BatchPack* pack = new BatchPack();

				union {
					thread_func i_func;
					void(__thiscall T::* o_func)(void*);
				};

				o_func = func;
				pack->func = i_func;
				pack->argument = argument;
				pack->finished = finished;
				pack->object = object;
				pack->size = size;
				pack->offset = offset;
				pack->array_proc = &array_processor<T>;
				array_pool.push({ pack, typeid(T).hash_code() });
			}
		};

		static Parallel parallel(4);
	}
}