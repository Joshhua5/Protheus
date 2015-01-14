#pragma once
#include "Context.h"
#include <AlignedBuffer.h>
#include <Buffer.h>

namespace Pro {
	namespace OpenCL { 
		class CLBuffer {
			cl_mem buffer_id;
		public:
			/*! Aligned buffer must have no padding */
			CLBuffer(const Context&, const Util::AlignedBuffer&, unsigned buf_flag = CL_MEM_READ_WRITE);
			CLBuffer(const Context&, const Util::Buffer&, unsigned buf_flag = CL_MEM_READ_WRITE);
			CLBuffer(const Context&, size_t size, unsigned buf_flag = CL_MEM_READ_WRITE);
			 
			CLBuffer(const CLBuffer&) = delete;
			CLBuffer& operator=(const CLBuffer&) = delete;

			CLBuffer(CLBuffer&&);
			CLBuffer& operator=(CLBuffer&&);

			cl_mem getID() const { return buffer_id; }
		};
	}
}