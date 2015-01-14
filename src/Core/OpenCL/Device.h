#pragma once
#include "extern\cl.h" 
#include <smart_pointer.h>

namespace Pro{
	namespace OpenCL{ 
		class Device{
			cl_device_id device;
			cl_command_queue queue;

		public:
			Device(cl_device_id device, cl_command_queue queue = nullptr){
				this->device = device;
				this->queue = queue;
			}

			cl_device_id getDevice() const { return device; }
			cl_command_queue getQueue() const { return queue; }
		};
	}
}