#include "extern\cl.h"
#include "Context.h"
#include "Device.h"
#include <ArrayList.h>
#include <Error.h>
#include <ErrorCheck.h>
#include <smart_pointer.h>

namespace Pro{
	namespace OpenCL{
		using namespace Util;
		 
		static void CL_CALLBACK cl_error_callback(const char* msg, const void* pvt_info, size_t cb, void* user_data){
			error.reportError(msg);
		}

		class Platform{ 
			ArrayList<Device> devices;
			 
		public:
			Platform(unsigned device_type = CL_DEVICE_TYPE_DEFAULT);

			ArrayList<Device>& getDevices();
		};
	}
}