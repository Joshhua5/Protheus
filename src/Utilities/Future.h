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

			/*! Yeilds the thread until the job has been completed */
			inline void Wait() const {
				while (!IsFinished()) { std::this_thread::yield(); }
			}

			//! Reset's the Future for reuse
			inline void Reset(const unsigned count) {
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