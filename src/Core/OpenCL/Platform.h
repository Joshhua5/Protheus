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

		class Platform{ 
			cl_uint device_count;
			cl_device_id* devices;
			cl_command_queue* queues;
			Context context;
			 
		public:
			Platform(unsigned device_type = CL_DEVICE_TYPE_DEFAULT);
			  
			/*! Returns nullptr if the index is out of range*/
			Device getDevice(unsigned index);

			const cl_device_id* getDevices() const{ return devices; }
			const cl_command_queue* getQueues() const { return queues; }
			const Context& getContext() const { return context; }
		};
	}
}