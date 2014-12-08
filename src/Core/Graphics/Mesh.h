#pragma once

#include <string> 
#include <memory>
#include <cassert>
#include <vector>
#include "Program.h"
#include "MeshObject.h"
#include "VertexArray.h"
#include "extern\glew.h"

namespace Pro { 
	using namespace std; 
	// All verticies are GL_FLOAT and elements are GLUINT
	class Mesh { 
		VertexArray vao;
		Mesh(const Mesh&) = delete;
		Mesh operator=(const Mesh&) = delete; 
	 
		std::vector<MeshObject> objects;
		GLuint verticies;
		GLuint elements;
		GLenum format;
		unsigned object_count = 0;
		bool vertexW;
		bool hasUV;
		bool hasNormals;
		
	public:
		Mesh(GLuint verticies, GLuint elements, GLenum format, bool vertexContainsW, bool hasUV , bool hasNormals);
		Mesh(Mesh&&);
		Mesh& operator=(Mesh&&);
		~Mesh();

		void attachObject(MeshObject&&);
		
		MeshObject* getObject(const string& object_name);

		inline void setVertexAttribute(const string& attrib_name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const unsigned offset);
		void bindProgram(const string& object_name, GLuint program_id);
		void bindProgram(GLuint program_id);

		inline void bind();
		inline void unbind();
		void draw();
	};
}