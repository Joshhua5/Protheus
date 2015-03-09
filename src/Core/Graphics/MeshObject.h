#pragma once


#include <string> 
#include <memory>
#include <vector>
#include "Program.h"
#include <Buffer.h>
#include <smart_ptr.h>
#include <BufferWriter.h>
#include "VertexArray.h"
#include "extern\glew.h"

namespace Pro {
	namespace Graphics {
		using namespace std;
		using namespace Util;

		enum struct FACE_FORMAT {
			UNDEFINED,
			VERTEX,
			VERTEX_NORMAL,
			VERTEX_UV_NORMAL,
			VERTEX_UV
		}; 

		struct MeshObject {
			string name;

			union {
				GLuint start;
				const void* p_start;
			};
			GLuint size = 0;
			unsigned char vertex_per_face = 0;
			unsigned char floats_per_vertex = 0;
			unsigned char tex_coord_per_vertex = 0;
			unsigned vertex_count = 0;

			bool has_tex_coord = false;
			bool has_normals = false;

			FACE_FORMAT face_format = FACE_FORMAT::UNDEFINED;


			MeshObject();
			MeshObject(const string& name, unsigned start, unsigned size);
			MeshObject(MeshObject&&);
			MeshObject& operator=(MeshObject&&);
			MeshObject(const MeshObject&);
			MeshObject& operator=(const MeshObject&);
			~MeshObject();

			bool     HasTexCoord() const;
			bool     HasNormals() const;
			GLuint   VertexSize() const;
			GLuint   NormalSize() const;
			GLuint   TexCoordSize() const;

			GLsizei  Stride() const;
			GLuint   NormalOffset() const;
			GLuint   VertexOffset() const;

			GLuint   TexCoordOffset() const;
			GLenum   GetMode() const;
		};

		//! A Mesh Object with temporary pointers, only used during Mesh Loading
		struct MeshObjectTemp : public MeshObject{
			MeshObjectTemp(const string& name, unsigned start, unsigned size) : MeshObject(name, start, size){}

			Buffer* temp = nullptr;
			BufferWriter* tempWriter = nullptr;

			//! Creates a MeshObject out the MeshObjectTemp
			MeshObject Export() const{
				return MeshObject(static_cast<MeshObject>(std::move(*this)));
			}
		};
	}
}