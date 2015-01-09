#include "CLProgram.h"

using namespace Pro;
using namespace OpenCL;

static void CL_CALLBACK compile_callback(cl_program, void* ptr) {
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
	program_id = clCreateProgramWithSource(platform.getContext().getContext(), 1, &ptr, NULL, NULL);
	finished.reset(1);
	clBuildProgram(program_id, NULL, platform.getDevices(), NULL, &compile_callback, &finished);
}

Program::Program(const Platform& platform, const char* buf) {
	program_id = clCreateProgramWithSource(platform.getContext().getContext(), 1, &buf, NULL, NULL);
	clBuildProgram(program_id, NULL, platform.getDevices(), NULL, &compile_callback, &finished); 
	finished.reset(1); 
}

Kernel Program::getKernel(const string& kernel_name) {
	return Kernel(clCreateKernel(program_id, kernel_name.data(), NULL));
}
