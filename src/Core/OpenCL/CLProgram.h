#pragma once 
#include "Kernel.h"
#include "Platform.h"
#include "..\FileSystem\CFile.h"
#include <Future.h>

namespace Pro {
	namespace OpenCL {
		/*! A collection of kernels 
		*/
		class Program {
			cl_program program_id;
			Future finished;
		public: 
			Program(const Program&) = delete;
			Program operator=(const Program&) = delete;
			Program(Program&&);
			Program& operator=(Program&&);
			Program(const Platform& platform, const Buffer&);
			Program(const Platform& platform, const char*);

			cl_program getProgram() const { return program_id; }

			Kernel getKernel(const string& kernel_name);
		};
	}
}