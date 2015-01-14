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
   
bool Kernel::setArgument(unsigned index, size_t size, const void* value) {
	return clSetKernelArg(kernel_id, index, size, value) == CL_SUCCESS; 
}

bool Kernel::setArgument(unsigned index, const CLBuffer& memory) { 
	return clSetKernelArg(kernel_id, index, sizeof(cl_mem), memory.getID()) == CL_SUCCESS;
}

bool Kernel::execute(const Device& device,
	unsigned work_dim,
	const size_t* g_work_size ,
	const size_t* l_work_size ,
	const size_t* g_work_offset  ,
	ArrayList<cl_event>* wait_list ,
	cl_event* event) {

	size_t wait_size = 0;
	cl_event* _wait_list = nullptr;
	cl_event* _event = nullptr;

	if (wait_list != nullptr) {
		wait_size = wait_list->size();
		_wait_list = wait_list->data();
	}  
	return clEnqueueNDRangeKernel(device.getQueue(), kernel_id, work_dim, g_work_offset, g_work_size, l_work_size, wait_size, _wait_list, event) == CL_SUCCESS;
}


bool Kernel::execute1D(const Device& device,
	const size_t g_work_size,
	const size_t l_work_size ,
	const size_t g_work_offset,
	ArrayList<cl_event>* wait_list ,
	cl_event* event) {
	return execute(device, 1, &g_work_size, &l_work_size, &g_work_offset, wait_list, event);
}

bool Kernel::execute2D(const Device& device,
	const Math::Vector2<size_t>& g_work_size,
	const Math::Vector2<size_t>& l_work_size,
	const Math::Vector2<size_t>& g_work_offset,
	ArrayList<cl_event>* wait_list ,
	cl_event* event) {
	return execute(device, 2, g_work_size.data(), l_work_size.data(), g_work_offset.data(), wait_list, event);
}

bool Kernel::execute3D(const Device& device,
	const Math::Vector3<size_t>& g_work_size,
	const Math::Vector3<size_t>& l_work_size,
	const Math::Vector3<size_t>& g_work_offset,
	ArrayList<cl_event>* wait_list,
	cl_event* event) {
	return execute(device, 3, g_work_size.data(), l_work_size.data(), g_work_offset.data(), wait_list, event);
}