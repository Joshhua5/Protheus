#pragma once
#include <FileSystem\FileSystem.h>
#include <OpenCL\Platform.h>
#include <OpenCL\Buffer.h>
#include "OpenCL\Device.h"
#include <OpenCL\CLProgram.h>

#include <BufferWriter.h>

using namespace Pro;
using namespace OpenCL;
 
const size_t array_size = 1E4;

int main() {
	FileSystem fs;

	Platform plat(CL_DEVICE_TYPE_GPU);

	Program program(plat, fs.getFile("Kernel/Test.cl"));

	Kernel kern = program.getKernel("test");
	 
	Device dev = plat.getDevice(0);

	Buffer arr1(sizeof(int) * array_size);
	Buffer arr2(sizeof(int) * array_size);

	BufferWriter w1(&arr1);
	BufferWriter w2(&arr2);

	for (size_t x = 0; x < array_size; ++x) {
		w1.write(x);
		w2.write(x + 1);
	}

	CLBuffer in1(plat.getContext(), arr1, CL_MEM_READ_ONLY);
	CLBuffer in2(plat.getContext(), arr2, CL_MEM_READ_ONLY);

	CLBuffer out(plat.getContext(), sizeof(int) * array_size, CL_MEM_WRITE_ONLY);

	kern.setArgument(0, in1);
	kern.setArgument(1, in2);
	kern.setArgument(2, out);
	 
	kern.execute1D(dev, 512, 4096, 0);

	clFlush(dev.getQueue());
	clFinish(dev.getQueue()); 

	return 0;
}