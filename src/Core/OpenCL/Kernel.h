#pragma once

#include <ArrayList.h>
#include <ProMath.h>
#include "extern\cl.h"
#include "Device.h" 
#include "Buffer.h"
#include "CLEvent.h"

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

			/*! Returns false if the argument wasn't valid
			*/
			bool setArgument(unsigned index, size_t size, const void* value);
			bool setArgument(unsigned index, const CLBuffer& memory);


			bool execute1D(const Device& device, 
							const size_t g_work_size = 512,
							const size_t l_work_size = 64,
							const size_t g_work_offset = 0,
							ArrayList<cl_event>* wait_list = nullptr,
							cl_event* event = nullptr);

			bool execute2D(const Device& device, 
							const Math::Vector2<size_t>& g_work_size,
							const Math::Vector2<size_t>& l_work_size,
							const Math::Vector2<size_t>& g_work_offset,
							ArrayList<cl_event>* wait_list = nullptr,
							cl_event* event = nullptr);

			bool execute3D(const Device& device, 
							const Math::Vector3<size_t>& g_work_size,
							const Math::Vector3<size_t>& l_work_size,
							const Math::Vector3<size_t>& g_work_offset,
							ArrayList<cl_event>* wait_list = nullptr,
							cl_event* event = nullptr);
			 
			/*! Default values are non optimal,
			Args:
				device - Device for the kernel to execut eon
				work_dim - Amount of dimensions
				g_work_offset - Global Size Offset
				g_work_size - Global Size
				l_work_size -  Local Size
				wait_list - List of events which must finish before the kernel executes
				event - An event which is set when the kernel finishes*/
			bool execute(const Device& device,
				unsigned work_dim,
				const size_t* g_work_size,
				const size_t* l_work_size,
				const size_t* g_work_offset = nullptr,
				ArrayList<cl_event>* wait_list = nullptr,
				cl_event* event = nullptr);
		};
	}
}