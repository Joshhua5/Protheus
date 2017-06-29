#pragma once
#include <Vector3.h>
#include <Vector2.h>
#include <Matrix44.h>
#include <Log.h>
#include <string>
#include "Program.h"
#include "OpenGL.h"

namespace Pro {
	namespace Graphics { 
		class VertexArray {
			GLuint vao_;
			GLuint preserved_vao_;

			VertexArray(const VertexArray&) = delete;
			VertexArray& operator=(const VertexArray&) = delete;
		public:
			VertexArray();
			VertexArray(VertexArray&&);
			VertexArray& operator=(VertexArray&&);
			~VertexArray();


			//! Optimisation is to have the vao bound before to prevent a preservedBind
            //! All size variables and offsets are in bytes
			void setVertexAttribute(const Program& program, const std::string& attrib_name,
				GLint size, GLenum type, GLboolean normalized, GLsizei Stride, const size_t offset);

            //! Binds the VAO
			void Bind() const;
            //! Unbinds the VAO to 0
			void Unbind() const;

            //! Binds the VAO and stores the currently bound VAO
			void PreservedBind();
            //! Unbinds the VAO and rebinds the VAO which was present at PreservedBind();
			void PreservedUnbind();
            
            //! Returns the VAO id
            GLuint id() const;
            
            //! Calling Initialise on a initialised vao will result in a no-op
            //! Purpose is to intialise a VAO that failed in the constructor
            void Initialise();
		};
	}
}