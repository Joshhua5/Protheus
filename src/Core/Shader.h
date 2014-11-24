#pragma once
#include <CBuffer.h>
#include <SDL_opengl.h>
#include <Error.h>

namespace Pro {
	class Shader
	{
		GLenum m_shader_type;
		GLuint m_shader_id;


		// uncopyable
		Shader(const Shader&);
		Shader& operator=(const Shader&);
	public:
		Shader(const CBuffer& shader, GLenum shader_type);
		~Shader();

		GLuint getShader() const;
	};
}
