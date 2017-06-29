#include "Profiler.h"

#ifdef WINDOWS
#include <Windows.h>
#endif

Pro::Profile::Profiler Pro::global_profiler;
 
using namespace Pro;
using namespace Profile;

static Log profile_log_("Profile.xml");
static std::atomic<std::chrono::milliseconds> poll_frequency_;
static std::atomic<bool> poll_gpu_;
static std::atomic<bool> poll_cpu_;
static std::atomic<bool> poll_net_;

static std::atomic<unsigned long> data_downloaded_;
static std::atomic<unsigned long> data_uploaded_;

static std::atomic<unsigned long> data_downloaded_ps;
static std::atomic<unsigned long> data_uploaded_ps;

struct CounterInfo{
	GLuint* counterList;
	int numCounters;
	int maxActiveCounters;
};

static void profile_net(std::atomic<bool>* profiling){
    unsigned long previous_download = 0;
    unsigned long previous_upload = 0;
    while(profiling->load()){
        data_downloaded_ps = data_downloaded_ - previous_download;
        data_uploaded_ps = data_uploaded_ - previous_upload;
            
        previous_upload = data_uploaded_;
        previous_download = data_downloaded_;
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

static void profile_AMD(std::atomic<bool>* profiling, std::atomic<bool>* paused){
#if defined(GL_AMD_performance_monitor)
	static unsigned monitor;
	glGenPerfMonitorsAMD(1, &monitor);

	GLint          n;
	GLuint        *groups;
	CounterInfo   *counters;

	glGetPerfMonitorGroupsAMD(&n, 0, NULL);
	groups = new GLuint[n];
	glGetPerfMonitorGroupsAMD(NULL, n, groups);
	 
	counters = new CounterInfo[n];
	for (int i = 0; i < n; i++)
	{
		glGetPerfMonitorCountersAMD(groups[i], &counters[i].numCounters,
			&counters[i].maxActiveCounters, 0, NULL);

		counters[i].counterList = (GLuint*) malloc(counters[i].numCounters *
			sizeof(int));

		glGetPerfMonitorCountersAMD(groups[i], NULL, NULL,
			counters[i].numCounters,
			counters[i].counterList);
	} 
#endif
	profile_log_.Report<LogCode::INFO>("", __FUNCTION__, __LINE__);

	while (profiling->load()){
		// Check if paused
		while (paused->load() && poll_gpu_.load()){ 
#ifdef GL_ATI_meminfo
			GLint vbo_free[4];
			GLint texture_free[4];
			GLint render_buffer_free[4];
			// Poll VRAM usage
			glGetIntegerv(GL_VBO_FREE_MEMORY_ATI, vbo_free);
			glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, texture_free);
			glGetIntegerv(GL_RENDERBUFFER_FREE_MEMORY_ATI, render_buffer_free);
#endif
			// Poll GPU usage


		}
		std::this_thread::sleep_for(poll_frequency_.load());
	}


#ifdef GL_AMD_performance_monitor
	glEndPerfMonitorAMD(monitor);
#endif
}

static void profile_INTEL(std::atomic<bool>* profiling, std::atomic<bool>* paused){}

static void profile_NVIDIA(std::atomic<bool>* profiling, std::atomic<bool>* paused){}

#ifdef WINDOWS
static void profile_WINDOWS(std::atomic<bool>* profiling, std::atomic<bool>* paused){

}
#endif

#ifdef LINUX
static void profile_LINUX(std::atomic<bool>* profiling, std::atomic<bool>* paused){
 
}
#endif

#ifdef APPLE
static void profile_APPLE(std::atomic<bool>* profiling, std::atomic<bool>* paused){

}
#endif

static void gpu_profile(std::atomic<bool>* profiling, std::atomic<bool>* paused){
    
    // Check the vendor
    while(poll_gpu_.load() == false)
        std::this_thread::sleep_for(std::chrono::seconds(1));
    
    const std::string vendor((const char*)glGetString(GL_VENDOR));
    
    if (vendor == "NVIDIA")
        profile_NVIDIA(profiling, paused);
    else if (vendor == "INTEL")
        profile_INTEL(profiling, paused);
    else if (vendor == "ATI Technologies")
        profile_AMD(profiling, paused);
    else
        global_log.Report<LogCode::FATAL>("OpenGL driver not present, possibly running on a software layer", vendor.data(), __LINE__);
}


static void initialise_profiler(std::atomic<bool>* profiling, std::atomic<bool>* paused){
    void(*os_profile)(std::atomic<bool>*, std::atomic<bool>*);

	// Get the OS specific profile function pointer
#ifdef WINDOWS
	os_profile = &profile_WINDOWS;
#elif LINUX
	os_profile = &profile_LINUX;
#elif APPLE
	os_profile = &profile_APPLE;
#endif

	// Profile the CPU
	std::thread(os_profile, profiling, paused).detach();

    // Start test for networking profile
    std::thread(gpu_profile, profiling, paused).detach();
    // Profile the Network
    std::thread(profile_net, profiling).detach();
} 

Profiler::Profiler(){
	running_.store(true);
	paused_.store(true);
    
    poll_gpu_ = false;
    poll_net_ = false;
    poll_cpu_ = false;
    
	poll_frequency_ = std::chrono::milliseconds(100);

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

void  Profiler::EnableCPUProfile(){
    poll_cpu_ = true;
}

void  Profiler::DisableCPUProfile(){
	poll_cpu_ = false;
}

void  Profiler::EnableGPUProfile(){
    if(Graphics::GL_CONTEXT_CREATED == false)
        global_log.Report<LogCode::FATAL>("Unable to start GPU profiling, no opengl context exists", __FUNCTION__, __LINE__);
	poll_gpu_ = true;
}

void Profiler::DisableGPUProfile(){
    poll_gpu_ = false;
}

void Profiler::EnableNetworkProfile() {
	poll_net_ = true;
}

void Profiler::DisableNetworkProfile(){
    poll_net_ = false;
}

void Profiler::RegisterNetworkDownload(unsigned bytes){
    data_downloaded_ += bytes;
}

void Profiler::RegisterNetworkUpload(unsigned bytes){
    data_uploaded_ += bytes;
}

void Profiler::PollFrequency(std::chrono::milliseconds intervals){
	poll_frequency_.store(intervals);
}

std::chrono::milliseconds Profiler::PollFrequency() const{
	return poll_frequency_.load();
}


unsigned long Profiler::DownloadBandwidth() {
	return data_downloaded_ps;
}

unsigned long Profiler::UploadBandwidth() {
	return data_uploaded_ps;
}