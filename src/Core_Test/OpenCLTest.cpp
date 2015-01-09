#include <FileSystem\FileSystem.h>
#include <OpenCL\Platform.h>
#include <OpenCL\CLProgram.h>

using namespace Pro;
using namespace OpenCL;
 

int main() {
	FileSystem fs;

	Platform plat(CL_DEVICE_TYPE_GPU);

	Program program(plat, fs.getFile("Kernel\test.cl"));

	Kernel kern = program.getKernel("test");
	 
}