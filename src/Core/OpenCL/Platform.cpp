#include "Platform.h"

using namespace Pro;
using namespace OpenCL;
using namespace Util;


Platform::Platform(unsigned device_type){
	ErrorCheck err(CL_SUCCESS);

	smart_pointer<cl_platform_id> platforms = nullptr;
	cl_uint platform_count = 0;

	// Get count of platforms to correctly allocate the platforms
	err = clGetPlatformIDs(0, 0, &platform_count);
	platforms = new cl_platform_id[platform_count];
	// Grab the platforms
	err = clGetPlatformIDs(platform_count, platforms._ptr, NULL);

	// Grab devices attached to platform
	smart_pointer<cl_device_id> device_ids = nullptr;
	cl_uint  device_count = 0;

	err = clGetDeviceIDs(platforms[0], device_type, 0, NULL, &device_count);
	device_ids = new cl_device_id[device_count];
	err = clGetDeviceIDs(platforms[0], device_type, device_count, device_ids._ptr, NULL);

	// Create a context for each device 
	cl_context context = clCreateContext(NULL, device_count, device_ids._ptr, &cl_error_callback, nullptr, NULL);
	 
	cl_int err_queue;
	for (unsigned x = 0; x < device_count; ++x){
		devices.emplace_back(clCreateCommandQueue(context, device_ids[x], NULL, &err_queue));
		err = err_queue;
	}

	if (err.hadError())
		error.reportError("Error in Pro::OpenCL::Platform");

}