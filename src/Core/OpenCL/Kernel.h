#pragma once

#include "extern\cl.h" 

namespace Pro {
	namespace OpenCL {
		class Kernel {
			cl_kernel kernel_id;
		public:
			Kernel();
			Kernel(cl_kernel);
			Kernel(const Kernel&) = delete;
			Kernel& operator=(const Kernel&) = delete;
			Kernel(Kernel&&);
			Kernel& operator=(Kernel&&); 
			 
			cl_kernel getKernel() const { return kernel_id; } 
		};
	}
}