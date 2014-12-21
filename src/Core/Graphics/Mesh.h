#pragma once

#include <string> 
#include <memory>
#include <cassert>
#include <vector>
#include "Program.h"
#include "MeshObject.h" 
#include "extern\glew.h"

namespace Pro {
	namespace Graphics {
		using namespace std;
		// All verticies are GL_FLOAT and elements are GLUINT
		class Mesh {
			Mesh(const Mesh&) = delete;
			Mesh operator=(const Mesh&) = delete;

			vector<MeshObject> objects;
			GLuint verticies;
			GLuint elements;
			unsigned object_count = 0;

		public:
			Mesh(GLuint verticies, GLuint elements);
			Mesh(Mesh&&);
			Mesh& operator=(Mesh&&);
			~Mesh();

			void attachObject(MeshObject&&);

			MeshObject* getObject(const string& object_name);
			vector<MeshObject>& getObjects();

			GLenum getMode() const;

			void bind() const;
			void unbind() const;
		};
	}
}