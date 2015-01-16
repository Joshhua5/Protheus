#include "Platform.h"

using namespace Pro;
using namespace OpenCL;
using namespace Util;

static void CL_CALLBACK cl_error_callback(const char* msg, const void* pvt_info, size_t cb, void* user_data) {
	error.reportError(msg);
}

Platform::Platform(unsigned device_type){
	ErrorCheck err(CL_SUCCESS);

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

	if (err.hadError())
		error.reportError("Error in Pro::OpenCL::Platform");

}


Device Platform::getDevice(unsigned index) {
	if (index > device_count) {
		error.reportError("Device index out of range, returning Device: 0");
		index = 0;
	}
	return Device(devices[index], queues[index]);
}