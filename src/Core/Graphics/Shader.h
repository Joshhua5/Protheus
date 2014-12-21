#pragma once 
#include <Buffer.h>
#include <Error.h>
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
			Shader(string shader, GLenum shader_type);
			~Shader();

			bool init(const Buffer& shader, GLenum shader_type);
			bool init(const string& shader, GLenum shader_type);
			GLuint getShader() const;
		};
	}
}