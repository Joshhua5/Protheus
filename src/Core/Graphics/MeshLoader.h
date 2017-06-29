#pragma once

#include <BufferReader.h>
#include <Vector3.h>  
#include <BufferWriter.h> 
#include <Vector4.h>  
#include <vector> 
#include "../FileSystem/File.h"
#include "Mesh.h"

namespace Pro {
	namespace Graphics {
		enum struct MODEL_FORMAT {
			OBJ,
			_3DS,
			UNDEFINED
		};

		class MeshLoader {
			//! Scans through whole file in search for file format features
            static MODEL_FORMAT QueryFormat(const Util::Buffer&);

		public:
			//! Loads a Object file
            static std::shared_ptr<Mesh> LoadOBJ(Buffer*);
			static std::shared_ptr<Mesh> Load3DSModel(const std::string& file_path);

			//! Queries for the file format and then loads the file
			//! Querying the file format can be a slow process depending on file type and 
			//! file size, Recommended to use the file type function if known eg LoadOBJ() 
            static std::shared_ptr<Mesh> LoadModel(Buffer&);
            static std::shared_ptr<Mesh> LoadModel(Buffer&&);

			//! Queries for the file format and then loads the file
			//! Querying the file format can be a slow process depending on file type and 
			//! file size, Recommended to use the file type function if known eg LoadOBJ() 
			static std::shared_ptr<Mesh> LoadModel(Buffer*);
            
            //! Generates and loads a mesh from a height map 
            //! Texture must be to the power of 2
			//! Arguments
			//!	size: Size of the pixel data in bytes
			//! offset: Offset of the pixel data in the stride
			//! stride: Size of the per pixel data in bytes
			//! For example a height map stored in the red channel of a RGB8 image would be (stride: 3, offset: 0, size: 1)
			//! or stored in the blue channel of a RGBA8 image (stride: 4, offset: 2, size: 1)
            static std::shared_ptr<Mesh> LoadMeshFromHeightMap(const Util::Buffer& map_data, const Vector2<unsigned>& map_dimensions, unsigned stride, unsigned offset, unsigned size);
		};
	}
}