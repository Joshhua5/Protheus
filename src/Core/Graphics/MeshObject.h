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
		string name;

		union  {
			GLuint start;
			const void* p_start;
		}; 
		GLuint size;

		MeshObject();
		MeshObject(const string& name, unsigned start, unsigned size);
		MeshObject(MeshObject&&);
		MeshObject& operator=(MeshObject&&);
		~MeshObject();
		 
	};
}