#pragma once

#include <Buffer.h>
#include <BufferReader.h>
#include <Vector3.h>
#include <BufferWriter.h>
#include <Vector4.h>
#include <Vector3.h>
#include <smart_pointer.h>
#include <vector>
#include "extern\glew.h"
#include "Mesh.h"

namespace Pro { 

	enum struct MODEL_FORMAT {
		OBJ,
		COLLADA,
		UNDEFINED
	};

	class MeshLoader {
		  
		static MODEL_FORMAT queryFormat(Buffer*);

	public:
		 
		static smart_pointer<Mesh> loadOBJ(Buffer*);
		static smart_pointer<Mesh> loadModel(Buffer*);
		static smart_pointer<Mesh> loadModel(Buffer&&);
	}; 
}