#pragma once

#include <Buffer.h>
#include <BufferReader.h>
#include <Vector3.h>
#include <BufferWriter.h>
#include <Vector4.h>
#include <Vector3.h>
#include <smart_ptr.h>
#include <vector>
#include "extern\glew.h"
#include "Mesh.h"

namespace Pro {
	namespace Graphics {
		using namespace Util;
		using namespace Math;
		enum struct MODEL_FORMAT {
			OBJ,
			COLLADA,
			UNDEFINED
		};

		class MeshLoader {

			static MODEL_FORMAT QueryFormat(Buffer*);

		public:

			static smart_ptr<Mesh> LoadOBJ(Buffer*);
			static smart_ptr<Mesh> LoadModel(Buffer*);
			static smart_ptr<Mesh> LoadModel(Buffer&&);
		};
	}
}