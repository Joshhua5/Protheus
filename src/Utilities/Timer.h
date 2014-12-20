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
	using namespace std;
	class Timer
	{
	private:
		unsigned long long startTick;
		unsigned long long lastTick;
		unsigned long long currentTick;
		unsigned long long high_resolution_clock_period;
	public:
		Timer();

		unsigned long long getTickDelta() const;
		double getTicksPerSec() const;
		void tick();
	}; 

	void Timer::tick() {
		lastTick = currentTick;
		// We reduce the accuracy of the timer by 1NS to prevent division by 0
		currentTick = std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}

	double Timer::getTicksPerSec() const {
		return (double)high_resolution_clock_period / ((currentTick - lastTick) + 1);
	}

	unsigned long long Timer::getTickDelta() const {
		return currentTick - lastTick;
	}

	Timer::Timer() {
		currentTick = std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch()).count();

		tick();
		high_resolution_clock_period = std::chrono::high_resolution_clock::period::den;
	} 

}