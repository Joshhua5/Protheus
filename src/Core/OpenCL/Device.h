#include "extern\cl.h"
#include <smart_pointer.h>

namespace Pro{
	namespace OpenCL{
		using namespace Util;
		class Device{
			cl_command_queue queue;

		public:
			Device(cl_command_queue queue = nullptr){
				this->queue = queue;
			}
		};
	}
}