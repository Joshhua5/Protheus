#pragma once

#include <string>
#include <vector>
#include "extern\glew.h"

namespace Pro { 
	struct MeshObject {
		std::string name;
		GLuint vba;
		GLuint start;
		GLuint size;

		MeshObject();
		MeshObject(MeshObject&&);
		~MeshObject(); 
	};

	// All verticies are GL_FLOAT and elements are GLUINT
	class Mesh { 
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
	
		void bind() const;
		void unbind() const;
		void draw() const;
	};
}