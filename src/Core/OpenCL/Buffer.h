#pragma once
#include "Context.h"
#include <AlignedBuffer.h>
#include <Buffer.h>

namespace Pro {
	namespace OpenCL { 
		class Buffer {
			cl_mem buffer_id;
		public:
			/*! Aligned buffer must have no padding */
			Buffer(const Context&, const Util::AlignedBuffer&, unsigned buf_flag = CL_MEM_READ_WRITE);
			Buffer(const Context&, const Util::Buffer&, unsigned buf_flag = CL_MEM_READ_WRITE);
			Buffer(const Context&, size_t size, unsigned buf_flag = CL_MEM_READ_WRITE);
			 
			Buffer(const Buffer&) = delete;
			Buffer& operator=(const Buffer&) = delete;

			Buffer(Buffer&&);
			Buffer& operator=(Buffer&&);

			cl_mem getID() const { return buffer_id; }
		};
	}
}