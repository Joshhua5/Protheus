#include "Profiler.h"

#ifdef WINDOWS
#include <Windows.h>
#endif

Pro::Profile::Profiler Pro::Profile::global_profiler;
std::atomic<Pro::Profile::ProfilerState> Pro::Profile::state;

using namespace Pro;
using namespace Profile;


static void profile_AMD(std::atomic<bool>* profiling, std::atomic<bool>* paused){
	static unsigned monitor;
	glGenPerfMonitorsAMD(1, &monitor);

}

static void profile_INTEL(std::atomic<bool>* profiling, std::atomic<bool>* paused){}

static void profile_NVIDIA(std::atomic<bool>* profiling, std::atomic<bool>* paused){}

static void profile_WINDOWS(std::atomic<bool>* profiling, std::atomic<bool>* paused){}

static void profile_LINUX(std::atomic<bool>* profiling, std::atomic<bool>* paused){}

static void profile_MAC(std::atomic<bool>* profiling, std::atomic<bool>* paused){}



static void initialise_profiler(std::atomic<bool>* profiling, std::atomic<bool>* paused){ 
		// Check the vendor
		const GLubyte* vendor = glGetString(GL_VENDOR);
	
		// Get the vendor specific profile function pointer
		void(*vendor_profile)(std::atomic<bool>*, std::atomic<bool>*);
	void(*os_profile)(std::atomic<bool>*, std::atomic<bool>*);

	if (vendor == "NVIDIA")
		vendor_profile = &profile_NVIDIA;
	else if (vendor == "INTEL")
		vendor_profile = &profile_INTEL;
	else if (vendor == "ATI Technologies")
		vendor_profile = &profile_AMD;
	else
		global_log.Report<LogCode::FATAL>("OpenGL driver not present, possibly running on a software layer", (const char*)vendor, __LINE__);

	// Get the OS specific profile function pointer
#ifdef WINDOWS
	os_profile = &profile_WINDOWS;
#elif LINUX
	os_profile = &profile_LINUX;
#elif MAC
	os_profile = &profile_MAC;
#endif
	 
	// Profile the CPU
	std::thread(os_profile, profiling, paused).detach();

	// Profile the GPU
	std::thread(vendor_profile, profiling, paused).detach();
}

void Profiler::StartProfiler(){ 
	std::thread(&initialise_profiler, &running_, &paused_).detach(); 
}

void Profiler::PauseProfiler(){
	paused_.store(true);
}

void Profiler::ResumeProfiler(){
	paused_.store(false);
}

void Profiler::StopProfiler(){
	running_.store(false);
}