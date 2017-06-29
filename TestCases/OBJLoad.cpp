#include <Graphics/Window.h>
#include <Graphics/Shader.h>
#include <FileSystem/FileSystem.h>
#include <Graphics/Program.h>
#include <Graphics/Camera.h>
#include <Graphics/MeshLoader.h>
#include <Graphics/Projection.h>
#include <thread> 
#include <Graphics/Lighting.h>
#include <Graphics/TextureLoader.h> 
#include <smart_ptr.h>
#include <Buffer.h>
#include <BufferWriter.h>
#include <Vector2.h>
#include <ArrayList.h>

using namespace Pro;
using namespace Util;
using namespace Input;
using namespace Graphics;

int main() {
	Graphics::Window window("OBJ Test", Vector2<int>(800, 600));

	FileSystem fs; 
	auto cube = MeshLoader::LoadModel(fs.GetFile("Models/standard_tile.obj")); 

	if (cube == nullptr) {
		global_log.Report<LogCode::FATAL>("Unable to load model", __FUNCTION__, __LINE__);
		global_log.Close();
		return 1;
	}
	return 0;
}