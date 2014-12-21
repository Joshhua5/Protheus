#include "VertexArray.h"

using namespace Pro;
using namespace Graphics;

static unsigned active_vertex_array = 0;

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
	preservedBind();
	GLint location = glGetAttribLocation(program.getID(), attrib_name.data());
	if (location == -1)
		error.reportErrorNR("Unable to locate shader attribute: " + attrib_name);
	if (size == 0)
		return;
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, size, type, normalized, stride, (void*)offset);
	preservedUnbind();
}

void VertexArray::bind() { 
	if (active_vertex_array == m_vao)
		return;
	active_vertex_array = m_vao;
	glBindVertexArray(m_vao);
}

void VertexArray::unbind() { 
	active_vertex_array = 0;
	glBindVertexArray(0);
} 

void VertexArray::preservedBind() {
	if (preserved_vao == m_vao)
		return;
	preserved_vao = active_vertex_array; 
	glBindVertexArray(m_vao);
}

void VertexArray::preservedUnbind() {
	if (preserved_vao == m_vao)
		return;
	glBindVertexArray(preserved_vao);
}
