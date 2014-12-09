#pragma once

#include <Vector3.h>
#include <Matrix44.h>
#include "extern\glew.h"
#include "Shader.h"

namespace Pro {
	class Program {
		GLuint program_id;

		// non copyable
		Program(const Program&) = delete;
		Program& operator=(const Program&) = delete;
	public:
		Program();
		~Program();
		Program(Program&&);
		Program& operator=(Program&&);

		void attachShader(const Shader& shader);

		void link();

		void setActive();
		GLuint getID();

		// If the stride is equal to 0 then the function call is silently returned and no state is changed
		void setVertexAttribute(const string& attrib_name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const unsigned offset);
		void setUniform(const string& uniform_name, const Vector3<float>& value);
		void setUniform(const string& uniform_name, GLint value);
		void setUniform(const string& uniform_name, const Matrix44<float>& value); 

		// If the stride is equal to 0 then the function call is silently returned and no state is changed
		static void setVertexAttribute(GLuint program_id, const string& attrib_name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const unsigned offset);
		static void setUniform(GLuint program_id, const string& uniform_name, const Vector3<float>& value);
		static void setUniform(GLuint program_id, const string& uniform_name, GLint value);
		static void setUniform(GLuint program_id, const string& uniform_name, const Matrix44<float>& value); 
		static void setUniform(GLuint program_id, const string& uniform_name, float* value, unsigned count);
	};
}