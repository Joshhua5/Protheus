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
namespace Pro{
	namespace Util{
		class Timer
		{
		private:
			unsigned long long startTick;
			unsigned long long lastTick;
			unsigned long long currentTick;
			double high_resolution_clock_period;
		public:
			Timer();
			~Timer();

			double getTickDelta();
			double getTicksPerSec();
			void tick();
		};
	}
}