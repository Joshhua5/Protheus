#pragma once

#include <atomic>
#include <thread>

namespace Pro {
	namespace Util {
		struct Future { 
			std::atomic<unsigned> finished_count;
			std::atomic<unsigned> worker_count;

			/*! Returns true if all threads have finished*/
			inline bool isFinished() const {
				return finished_count.load() == worker_count.load();
			}

			/*! Thread will call this function to flag it's completion */
			inline void thread_finished() {
				++finished_count;
			}

			/*! Waits until the job has been completed */
			inline void wait() {
				while (!isFinished()) { std::this_thread::yield(); }
			}

			inline void reset(unsigned count) {
				finished_count = 0;
				worker_count = count;
			}

			Future& operator=(const Future& rhs) {
				finished_count = rhs.finished_count.load();
				worker_count = rhs.worker_count.load();
				return *this;
			}
			 
		};
	}
}