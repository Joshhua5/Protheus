#include "VertexArray.h"

using namespace Pro;

VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_vao);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_vao);
}

VertexArray::VertexArray(VertexArray&& rhs) {
	m_vao = rhs.m_vao;
	rhs.m_vao = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& rhs) {
	m_vao = rhs.m_vao;
	rhs.m_vao = 0;
	return *this;
}
 
void VertexArray::setVertexAttribute(Program& program, const string& attrib_name,
	GLint size, GLenum type, GLboolean normalized, GLsizei stride, const unsigned offset) {

	program.setVertexAttribute(attrib_name, size, type, normalized, stride, offset);
}

void VertexArray::bind() {
	glBindVertexArray(m_vao);
}

void VertexArray::unbind() {
	glBindVertexArray(0);
}
