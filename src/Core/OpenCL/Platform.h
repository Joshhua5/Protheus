#pragma once

#include "extern\cl.h"
#include "Context.h"
#include "Device.h"
#include <ArrayList.h>
#include <Error.h>
#include <ErrorCheck.h>
#include <smart_pointer.h>

namespace Pro{
	namespace OpenCL{ 
		 
		static void CL_CALLBACK cl_error_callback(const char* msg, const void* pvt_info, size_t cb, void* user_data){
			error.reportError(msg);
		}

		class Platform{ 
			cl_device_id* devices;
			cl_command_queue* queues;
			Context context;
			 
		public:
			Platform(unsigned device_type = CL_DEVICE_TYPE_DEFAULT);

			const cl_device_id* getDevices() const{ return devices; }
			const cl_command_queue* getQueues() const { return queues; }
			const Context& getContext() const { return context; }
		};
	}
}