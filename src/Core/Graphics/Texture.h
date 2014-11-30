#pragma once

#include "extern\glcorearb.h"
#include <Vector2.h>

namespace Pro{
	using namespace Math;
	class Texture
	{
	private:
		GLuint texture_id;
		Vector2 dimensions;

		// prevent copying of a texture
		// leave undefined
		Texture(const Texture&); 
		Texture& operator=(const Texture&);
	public:
		Texture(const GLuint texture_id, const Vector2& dim); 
		~Texture();

		Texture(Texture&&);
		Texture& operator=(Texture&&);

		const GLuint getTextureID() const;
	}; 
}