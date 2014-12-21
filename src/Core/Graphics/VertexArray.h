#pragma once
#include <Vector3.h>
#include <Vector2.h>
#include <Matrix44.h>
#include <Error.h>
#include <string>

#include "Program.h"
#include "extern\glew.h"

namespace Pro {
	namespace Graphics {
		using namespace std;
		class VertexArray {
			GLuint m_vao;
			GLuint preserved_vao;

			VertexArray(const VertexArray&) = delete;
			VertexArray& operator=(const VertexArray&) = delete;
		public:
			VertexArray();
			VertexArray(VertexArray&&);
			VertexArray& operator=(VertexArray&&);
			~VertexArray();


			// Internally calls Program::setVertexAttribute with the same arguments
			// Optimisation is to have the vao bound before to prevent a preservedBind
			void setVertexAttribute(const Program& program, const string& attrib_name,
				GLint size, GLenum type, GLboolean normalized, GLsizei stride, const unsigned offset);

			void bind();
			void unbind();

			void preservedBind();
			void preservedUnbind();
		};
	}
}