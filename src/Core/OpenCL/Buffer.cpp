#include "Buffer.h"

using namespace Pro;
using namespace OpenCL;


OpenCL::CLBuffer::CLBuffer(const Context& context, Util::AlignedBuffer& buf, unsigned flag) {
	if (buf.IsPadded())
		log.Report<LogCode::ERROR>("Unable to create buffer, Alignedbuffer is padded (Check the Alignment and object size)", __FUNCTION__, __LINE__);
	else
		buffer_id = clCreateBuffer(context.getContext(), flag, buf.size(), buf.data(), NULL);
}
				  
OpenCL::CLBuffer::CLBuffer(const Context& context, Util::Buffer& buf, unsigned flag) {
	buffer_id = clCreateBuffer(context.getContext(), flag, buf.size(), buf.data(), NULL);
}
				 
OpenCL::CLBuffer::CLBuffer(const Context& context, size_t size, unsigned flag) {
	buffer_id = clCreateBuffer(context.getContext(), flag, size, NULL, NULL);
}
				 
OpenCL::CLBuffer::CLBuffer(CLBuffer&& rhs) {
	buffer_id = rhs.buffer_id;
	rhs.buffer_id = 0;
}
		
OpenCL::CLBuffer& OpenCL::CLBuffer::operator=(CLBuffer&& rhs) {
	buffer_id = rhs.buffer_id;
	rhs.buffer_id = 0;
	return *this;
}