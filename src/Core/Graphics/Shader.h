#pragma once 
#include <CBuffer.h>
#include <Error.h>
#include "extern\glew.h"

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
		Shader(string shader, GLenum shader_type);
		~Shader();

		bool init(const CBuffer& shader, GLenum shader_type); 
		bool init(const char* shader, GLenum shader_type);
		GLuint getShader() const;
	};
} 