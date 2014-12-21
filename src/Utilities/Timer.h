/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
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
#include <string>

namespace Pro {
	namespace Util {
		using namespace std;
		/*! Timer class will keep track of ticks and time
			Resolution of of nanoseconds
		*/
		class Timer
		{
		private:
			unsigned long long startTick;
			unsigned long long lastTick;
			unsigned long long currentTick;
			unsigned long long high_resolution_clock_period;
		public:
			Timer() {
				currentTick = std::chrono::duration_cast<std::chrono::nanoseconds>(
					std::chrono::high_resolution_clock::now().time_since_epoch()).count();
				tick();
				high_resolution_clock_period = std::chrono::high_resolution_clock::period::den;
			}

			/*! Get time since last tick*/
			unsigned long long getTickDelta() const {
				return currentTick - lastTick;
			}

			/*! Count how many ticks occur per second, value is extrapolated */
			double getTicksPerSec() const {
				// We reduce the accuracy of the timer by 1NS to prevent division by 0
				return (double)high_resolution_clock_period / ((currentTick - lastTick) + 1);
			}

			/*! Update the timer with a new tick*/
			void tick() {
				lastTick = currentTick;
				currentTick = std::chrono::duration_cast<std::chrono::nanoseconds>(
					std::chrono::high_resolution_clock::now().time_since_epoch()).count();
			}

		};
	}
}