#pragma once

#include "extern\glew.h"
#include <Vector2.h>

namespace Pro{ 
	class Texture
	{ 
		GLuint texture_id; 
		Vector2<unsigned> dimensions;

		// prevent copying of a texture
		// leave undefined
		Texture(const Texture&); 
		Texture& operator=(const Texture&);
	public:
		Texture(const GLuint texture_id, const Vector2<unsigned>& dim); 
		~Texture();

		Texture(Texture&&);
		Texture& operator=(Texture&&);

		const GLuint getTextureID() const;
		Vector2<unsigned>& getDimensions();

		void setFilter(GLenum type);
		void setBorder(GLenum type);
		void bind() const;
		void unbind() const;   
	}; 
}