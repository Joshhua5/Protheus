#pragma once
#include "OpenGL.h"
#include <Vector2.h>

namespace Pro {
	namespace Graphics {
		using namespace Math;
		class Texture
		{
			GLuint texture_id_;
			Vector2<unsigned> dimensions_;
			 
			// leave undefined
			Texture(const Texture&) = delete;
			Texture& operator=(const Texture&) = delete;
		public: 
            Texture(const Vector2<unsigned>& dim, GLenum format, GLenum type);
            Texture(const GLuint texture_id, const Vector2<unsigned>& dim);
			~Texture();

			Texture(Texture&&);
			Texture& operator=(Texture&&);

			const GLuint textureID() const;
			Vector2<unsigned>& dimensions();

			void SetFilter(GLenum type);
			void SetBorder(GLenum type);
			void Bind() const;
			void Unbind() const;
		};
	}
}
