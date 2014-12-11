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
 
void VertexArray::setVertexAttribute(const Program& program, const string& attrib_name,
	GLint size, GLenum type, GLboolean normalized, GLsizei stride, const unsigned offset) {
	GLint location = glGetAttribLocation(program.getID(), attrib_name.data());
	if (location == -1)
		error.reportErrorNR("Unable to locate shader attribute: " + attrib_name);
	if (size == 0)
		return;
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, size, type, normalized, stride, (void*)offset);
}

void VertexArray::bind() {
	glBindVertexArray(m_vao);
}

void VertexArray::unbind() {
	glBindVertexArray(0);
} 

void VertexArray::preservedBind() {
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&preserved_vao); 
	glBindVertexArray(m_vao);
}

void VertexArray::preservedUnbind() {
	glBindVertexArray(preserved_vao);
}
