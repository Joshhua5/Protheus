#pragma once

#include <string> 
#include <memory>
#include <cassert>
#include <vector>
#include "Program.h"
#include "MeshObject.h" 
#include "extern\glew.h"

namespace Pro { 
	using namespace std; 
	// All verticies are GL_FLOAT and elements are GLUINT
	class Mesh {  
		Mesh(const Mesh&) = delete;
		Mesh operator=(const Mesh&) = delete; 
	 
		vector<MeshObject> objects;
		GLuint verticies;
		GLuint elements;
		GLenum mode;
		unsigned object_count = 0;
		bool vertex_w;
		bool tex_coord_w;
		bool has_tex_coord;
		bool has_normals;
		
	public:
		Mesh(GLuint verticies, GLuint elements, GLenum mode, bool vertexContainsW, bool tex_coord_w, bool hasUV , bool hasNormals);
		Mesh(Mesh&&);
		Mesh& operator=(Mesh&&);
		~Mesh();

		void attachObject(MeshObject&&);
		
		MeshObject* getObject(const string& object_name);
		vector<MeshObject>& getObjects();

		GLenum getMode() const;

		void bind() const;
		void unbind() const;
		bool hasTexCoord() const;
		bool hasNormals() const; 

		GLuint normalSize() const;
		GLuint texCoordSize() const;
		GLuint vertexSize() const; 

		GLsizei stride() const; 

		GLuint normalOffset() const;
		GLuint vertexOffset() const;
		GLuint texCoordOffset() const;

	};
}