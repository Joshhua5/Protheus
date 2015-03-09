/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:12:2014 Waring J.

*************************************************************************/

#include "Log.h"
#include "Timer.h"
#include "ProfilerState.h"

#include "../Graphics/extern/glew.h"

#include <atomic>


namespace Pro{
	namespace Profile{
		enum struct ProfilerMode{
			CPU_PROFILE = 1 << 0,
			GPU_PROFILE = 1 << 1
		}; 

		extern std::atomic<ProfilerState> state;
		std::atomic<bool> running_;
		std::atomic<bool> paused_;

		class Profiler{ 
		public:
			void StartProfiler();

			void PauseProfiler();

			void ResumeProfiler();

			void StopProfiler();

			void ChangeProfilerMode(ProfilerMode);
		};

		extern Profiler global_profiler;
	}
} 