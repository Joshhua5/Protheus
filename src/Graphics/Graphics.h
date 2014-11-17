#pragma once 

#include <DataStructures.h> 
#include <Vector2.h>
#include "Window.h"
#include "ITextureLoader.h"

class Shader;
class Texture;
enum ShaderType; 

namespace Pro{
	namespace Graphics{
		using namespace Math;
		  
		Window createWindow(Vector2& position, Vector2& dimensions);

		Texture loadTexture(ITextureLoader*, CBuffer& file);

		Shader loadShader(ShaderType type, CBuffer& file);

	}
}