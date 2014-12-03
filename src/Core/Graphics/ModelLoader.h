#pragma once

#include <CBuffer.h>
#include <BufferReader.h>
#include <Vector3.h>
#include <BufferWriter.h>
#include <vector>
#include "Model.h"

namespace Pro { 

	enum struct MODEL_FORMAT {
		OBJ,
		UNDEFINED
	};

	class ModelLoader {
		  
		static MODEL_FORMAT queryFormat(CBuffer*);

	public:
		 
		static Model* loadOBJ(CBuffer*);
		static Model* loadModel(CBuffer*);
		static Model* loadModel(CBuffer&&); 
	}; 
}