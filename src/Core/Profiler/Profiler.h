/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:12:2014 Waring J.

*************************************************************************/
#pragma once

#include "Log.h"
#include "Timer.h" 
#include "../Graphics/OpenGL.h"

#include <atomic>


namespace Pro{
	namespace Profile{  
		class Profiler{
			std::atomic<bool> running_;
			std::atomic<bool> paused_;
		public:
			Profiler();

			//! Pauses all profiling
			void PauseProfiler();

			//! Resumes the profiler back to it's original resumed state
			//! GPU or CPU profiling will remain paused if DisableCPUProfile or DisableGPUProfile was called
			void ResumeProfiler();

			//! Once stopped, the profiler can't be started
			//! refer to Profile::PauseProfiler
			void StopProfiler();

			//! Resumes the CPU profiler
			void EnableCPUProfile();

			//! Pauses the CPU profiler
			void DisableCPUProfile();

			//! Resumes the GPU profiler
			void EnableGPUProfile();

			//! Pauses the GPU profiler
			void DisableGPUProfile();
            
            //! Resumes the Network profiler
            void EnableNetworkProfile();
            
            //! Pauses the Network profiler
            void DisableNetworkProfile();

			//! Sets the time between performance queries
			void PollFrequency(std::chrono::milliseconds intervals);

			//! Returns the time between performance queries
			std::chrono::milliseconds PollFrequency() const;
            
			//! Registers network usage upload
            void RegisterNetworkDownload(unsigned bytes);
            
			//! Registers network usage upload
            void RegisterNetworkUpload(unsigned bytes);
            
			//! Returns the current bandwidth in bytes/s
            unsigned long DownloadBandwidth();
            
			//! Returns the current bandwidth in bytes/s
            unsigned long UploadBandwidth();
		};
	}
    extern Profile::Profiler global_profiler;
}