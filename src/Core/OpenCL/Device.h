#include "extern\cl.h"
#include <smart_pointer.h>

namespace Pro{
	namespace OpenCL{
		using namespace Util;
		class Device{
			cl_command_queue queue;
			Device(cl_command_queue queue){
				this->queue = queue;
			}
		};
	}
}