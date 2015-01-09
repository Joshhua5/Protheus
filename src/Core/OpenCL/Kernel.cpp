#include "Kernel.h"

using namespace Pro;
using namespace OpenCL;

Kernel::Kernel(Kernel&& rhs) {
	kernel_id = rhs.kernel_id;
	rhs.kernel_id = 0;
}

Kernel& Kernel::operator=(Kernel&& rhs) {
	kernel_id = rhs.kernel_id;
	rhs.kernel_id = 0;
	return *this;
} 

Kernel::Kernel() {
	kernel_id = 0;
}  

Kernel::Kernel(cl_kernel id) {
	kernel_id = id;
}
 