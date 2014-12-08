#pragma once


#include <string> 
#include <memory>
#include <vector>
#include "Program.h"
#include "VertexArray.h"
#include "extern\glew.h"

namespace Pro {
	using namespace std;
	struct MeshObject {
		GLuint program;
		string name;


		std::vector<std::pair<GLuint, GLuint>> uniforms1u;
		std::vector<std::pair<GLuint, Vector3<float>>> uniforms3f;
		Matrix44<float> model_matrix;
		GLuint start;
		GLuint size;

		MeshObject();
		MeshObject(MeshObject&&);
		MeshObject& operator=(MeshObject&&);
		~MeshObject();


		void setUniforms() const;
		void setVertexAttribute(const string& attrib_name, GLint size,
			GLenum type, GLboolean normalized, GLsizei stride, const unsigned offset) const;
		void setUniform(const string& uniform_name, const Vector3<float>& value);
		void setUniform(const string& uniform_name, GLint value);
		void setModelMatrix(const Matrix44<float>& matrix);
	};
}