/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:
	Counts the delta between ticks in nanaseconds and provides data
	abount the tick times

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <chrono> 
#include <thread>
#include <string>

namespace Pro {
	namespace Util {
		/*! Timer class will keep track of ticks and time
			Resolution of of nanoseconds
		*/
		template<class T = std::chrono::nanoseconds>
		class Timer
		{ 
			unsigned long long start_tick_;
			unsigned long long last_tick_;
			unsigned long long current_tick_;
			unsigned long long high_resolution_clock_period_;
		public:
			Timer() {
				current_tick_ = std::chrono::duration_cast<T>(
					std::chrono::high_resolution_clock::now().time_since_epoch()).count();
				Tick();
				high_resolution_clock_period_ = std::chrono::high_resolution_clock::period::den;
			}

			/*! Get time since last tick*/
			unsigned long long GetTickDelta() const {
				return current_tick_ - last_tick_;
			}

			/*! Count how many ticks occur per second, value is extrapolated */
			//TODO values are incorrect for anything other than nanoseconds

			double GetTicksPerSec() const {
				return (current_tick_ - last_tick_) ? 1E9 : 1E9 / (current_tick_ - last_tick_);
			}

			/*! Update the timer with a new tick*/
			unsigned long long Tick() {
				last_tick_ = current_tick_;
				return current_tick_ = std::chrono::duration_cast<T>(
					std::chrono::high_resolution_clock::now().time_since_epoch()).count();
			}

			static long long getTime() {
				return std::chrono::duration_cast<T>(
					std::chrono::high_resolution_clock::now().time_since_epoch()).count();
			}

			//! Accepts a std::chrono::duration
			template<class TIME>
			static long long GetTime(){ 
				return std::chrono::duration_cast<TIME>(
					std::chrono::high_resolution_clock::now().time_since_epoch()).count();
			}
            
            
		};
	}
    
    static inline Util::Timer<std::chrono::nanoseconds>& GetGlobalTimer(){
        static Util::Timer<std::chrono::nanoseconds> global;
        return global;
    }
    
    //! This function exists so C code can suspend it's thread with millisecond accuracy
    static inline void sleep_for(unsigned milliseconds){
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
}

