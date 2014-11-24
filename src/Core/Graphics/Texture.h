#pragma once

#include <SDL_opengl.h>
#include <Vector2.h>

namespace Pro{
	using namespace Math;
	class Texture
	{
	private:
		GLuint texture_id;
	public:
		Texture(GLuint texture_id, Vector2 dim);
		~Texture();
	};

}