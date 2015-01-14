#include "CLProgram.h"

using namespace Pro;
using namespace OpenCL;

static void CL_CALLBACK compile_callback(cl_program, void* ptr) {
	if(ptr != nullptr)
		static_cast<Future*>(ptr)->thread_finished(); 
}

Program::Program(Program&& rhs) { 
	program_id = rhs.program_id;
	finished = rhs.finished;
	rhs.program_id = 0;
}

Program& Program::operator=(Program&& rhs) {
	program_id = rhs.program_id;
	finished = rhs.finished;
	rhs.program_id = 0;
	return *this;
}
Program::Program(const Platform& platform, const Buffer& buffer) {
	const char* ptr = buffer.data<char>();
	cl_int err = CL_SUCCESS;
	size_t size = buffer.size();
	program_id = clCreateProgramWithSource(platform.getContext().getContext(), 1, &ptr, &size, &err);
	finished.reset(1);
	if (err != CL_SUCCESS)
		error.reportError("Unable to CreateProgramWithSource, Error Code: " + err);
	err = clBuildProgram(program_id, NULL, NULL, NULL, NULL, NULL); // &compile_callback, &finished);
	if (err != CL_SUCCESS)
		error.reportError("Unable to clBuildProgram, Error Code: " + err);
}

Program::Program(const Platform& platform, const char* buf) { 
	cl_int err;
	program_id = clCreateProgramWithSource(platform.getContext().getContext(), 1, &buf, NULL, &err);
	clBuildProgram(program_id, NULL, NULL, NULL, &compile_callback, &finished); 
	finished.reset(1); 
}

Kernel Program::getKernel(const string& kernel_name) {
	return Kernel(clCreateKernel(program_id, kernel_name.data(), NULL));
}