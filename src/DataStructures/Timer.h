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

namespace Pro{
	namespace Util{
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
	}
}