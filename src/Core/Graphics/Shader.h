#pragma once 
#include <Buffer.h>
#include <Log.h>
#include "OpenGL.h"

namespace Pro {
	namespace Graphics {
		class Shader
		{
			GLenum m_shader_type;
			GLuint m_shader_id;

			// uncopyable
			Shader(const Shader&) = delete;
			Shader& operator=(const Shader&) = delete;
		public:
			Shader();
            Shader(const Util::Buffer& shader, GLenum shader_type); 
			Shader(const std::string& shader, GLenum shader_type);
			~Shader();

            bool Init(const Util::Buffer& shader, GLenum shader_type);
			bool Init(const std::string& shader, GLenum shader_type);
			GLuint GetShader() const;
            bool Validate() const;
		};
	}
}