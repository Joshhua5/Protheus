#pragma once

#include <atomic>
#include <thread>

namespace Pro {
	namespace Util {
		class Future { 
			std::atomic<unsigned> finished_count_;
			std::atomic<unsigned> worker_count_;
		public:
			/*! Returns true if all threads have finished*/
			inline bool IsFinished() const {
				return finished_count_.load() == worker_count_.load();
			}

			/*! Thread will call this function to flag it's completion */
			inline void ThreadFinished() {
				++finished_count_;
			}

			/*! Waits until the job has been completed */
			inline void Wait() {
				while (!IsFinished()) { std::this_thread::yield(); }
			}

			inline void Reset(unsigned count) {
				finished_count_ = 0;
				worker_count_ = count;
			}

			Future& operator=(const Future& rhs) {
				finished_count_ = rhs.finished_count_.load();
				worker_count_ = rhs.worker_count_.load();
				return *this;
			}
			 
		};
	}
}