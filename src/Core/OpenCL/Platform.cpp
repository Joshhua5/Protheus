#include "Platform.h"

using namespace Pro;
using namespace OpenCL;
using namespace Util;

static void CL_CALLBACK cl_error_callback(const char* msg, const void* pvt_info, size_t cb, void* user_data) {
	Pro::global_log.Report<LogCode::ERROR>(msg, __FUNCTION__, __LINE__);
}

Platform::Platform(unsigned device_type){ 
	ErrorCheck<int> err(CL_SUCCESS);

	smart_array<cl_platform_id> platforms = nullptr;
	cl_uint platform_count = 0;

	// Get count of platforms to correctly allocate the platforms
	err = clGetPlatformIDs(0, 0, &platform_count);
	platforms = new cl_platform_id[platform_count];
	// Grab the platforms
	err = clGetPlatformIDs(platform_count, platforms.get(), NULL);

	// Grab devices attached to platform 

	err = clGetDeviceIDs(platforms[0], device_type, 0, NULL, &device_count);
	devices = new cl_device_id[device_count];
	err = clGetDeviceIDs(platforms[0], device_type, device_count, devices, NULL);

	// Create a context for each device
	context = clCreateContext(NULL, device_count, devices, &cl_error_callback, nullptr, NULL);
	 
	cl_int err_queue;

	// Create Command Queues
	queues = new cl_command_queue[device_count];

	for (unsigned x = 0; x < device_count; ++x){
		queues[x] = clCreateCommandQueue(context.getContext(), devices[x], NULL, &err_queue);
		err = err_queue;
	}

	if (err.HadError())
		global_log.Report<LogCode::ERROR>("Error in Pro::OpenCL::Platform", __FUNCTION__, __LINE__);

}


Device Platform::getDevice(unsigned index) {
	if (index > device_count) {
		global_log.Report<LogCode::ERROR>("Device index out of range, returning Device: 0", __FUNCTION__, __LINE__);
		index = 0;
	}
	return Device(devices[index], queues[index]);
}