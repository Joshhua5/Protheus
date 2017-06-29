/*************************************************************************
 Protheus Source File.
 Copyright (C), Protheus Studios, 2013-2016.
 -------------------------------------------------------------------------
 
 Description:
 
 -------------------------------------------------------------------------
 History:
 - 6:5:2015 Waring J.
 
 *************************************************************************/


#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>

namespace Pro {
	namespace Util { 
		//! A Sync is used to return a value from a thread
		//! or sync threads together
		//! The last thread to return it's result 
		class Sync { 
			std::atomic<unsigned short> finished_count_;
			std::atomic<unsigned short> worker_count_;
            std::mutex cv_mutex;
            std::condition_variable cv;
		public:
			Sync(){ 
				finished_count_.store(0);
				worker_count_.store(0);
			}

			//! Returns true if all threads have finished
			inline bool IsFinished() const {
				return finished_count_.load() == worker_count_.load();
			}

			//! Thread will call this function to flag it's completion
			inline void ThreadFinished() {
                std::unique_lock<std::mutex> lock(cv_mutex);
				finished_count_.fetch_add(1);
                if(IsFinished())
                    cv.notify_all();
			}
 
            //! Waits for the sync to finish, will break every @timeout period and
            //! return false if the condition is still false
            inline bool Wait(std::chrono::milliseconds timeout = std::chrono::milliseconds(1000)) {
				if(IsFinished())
					return true;
                
                std::unique_lock<std::mutex> lock(cv_mutex);
                cv.wait_for(lock, timeout, [this]{
                    return IsFinished();
                });
                
                return IsFinished();
			}
			
			//! Adds a worker to the future
			inline void AddWorker(const unsigned count) {
				worker_count_.fetch_sub(finished_count_);
				finished_count_ = 0;
				worker_count_.fetch_add(count);
			}

			inline void workerCount(const unsigned short count){
				worker_count_.store(count);
			} 
			
			inline unsigned short workerCount(){
				return worker_count_.load();
			}
			 
			inline Sync& operator=(const Sync& rhs) {
				finished_count_ = rhs.finished_count_.load();
				worker_count_ = rhs.worker_count_.load();
				return *this;
			} 
		};
	}
}