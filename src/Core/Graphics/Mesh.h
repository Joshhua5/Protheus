#pragma once

#include <string> 
#include <memory>
#include <cassert>
#include <vector>
#include "Program.h"
#include "MeshObject.h" 
#include "OpenGL.h"

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

			void AttachObject(MeshObject&&); 
			MeshObject* object(const string& object_name);
			vector<MeshObject>& GetObjects();
			const vector<MeshObject>& GetObjects() const;

			GLenum GetMode() const;

			void Bind() const;
			void Unbind() const;
		};
	}
}