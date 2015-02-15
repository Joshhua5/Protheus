#pragma once 
#include <Buffer.h>
#include <Log.h>
#include "extern\glew.h"

namespace Pro {
	namespace Graphics {
		using namespace Util;
		class Shader
		{
			GLenum m_shader_type;
			GLuint m_shader_id;

			// uncopyable
			Shader(const Shader&) = delete;
			Shader& operator=(const Shader&) = delete;
		public:
			Shader();
			Shader(const Buffer& shader, GLenum shader_type);
			Shader(Buffer&& shader, GLenum shader_type);
			Shader(const std::string& shader, GLenum shader_type);
			~Shader();

			bool Init(const Buffer& shader, GLenum shader_type);
			bool Init(const std::string& shader, GLenum shader_type);
			GLuint GetShader() const;
		};
	}
}