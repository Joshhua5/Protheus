#include "Buffer.h"

using namespace Pro;
using namespace OpenCL;


OpenCL::Buffer::Buffer(const Context& context, const Util::AlignedBuffer& buf, unsigned flag) {
	if (buf.isPadded())
		error.reportErrorNR("Unable to create buffer, Alignedbuffer is padded (Check the Alignment and object size)");
	else
		buffer_id = clCreateBuffer(context.getContext(), flag, buf.size(), buf.data(), NULL);
}

OpenCL::Buffer::Buffer(const Context& context, const Util::Buffer& buf, unsigned flag) {
	buffer_id = clCreateBuffer(context.getContext(), flag, buf.size(), buf.data(), NULL);
}

OpenCL::Buffer::Buffer(const Context& context, size_t size, unsigned flag) {
	buffer_id = clCreateBuffer(context.getContext(), flag, size, NULL, NULL);
}

OpenCL::Buffer::Buffer(Buffer&& rhs) {
	buffer_id = rhs.buffer_id;
	rhs.buffer_id = 0;
}

OpenCL::Buffer& OpenCL::Buffer::operator=(Buffer&& rhs) {
	buffer_id = rhs.buffer_id;
	rhs.buffer_id = 0;
	return *this;
}