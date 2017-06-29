#include "VertexArray.h"

using namespace std;
using namespace Pro;
using namespace Graphics; 

static unsigned active_vertex_array = 0;

VertexArray::VertexArray() {
	glGenVertexArrays(1, &vao_);
}

VertexArray::~VertexArray() {
	if (vao_ != 0)
		glDeleteVertexArrays(1, &vao_);
}

VertexArray::VertexArray(VertexArray&& rhs) {
	vao_ = rhs.vao_;
	rhs.vao_ = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& rhs) {
	vao_ = rhs.vao_;
	rhs.vao_ = 0;
	return *this;
}

void VertexArray::setVertexAttribute(const Program& program, const string& attrib_name,
	GLint size, GLenum type, GLboolean normalized, GLsizei stride, const size_t offset) {
	if (vao_ == 0) {
		global_log.Report<LogCode::FAULT>("setVertexAttribute called on a invalid VAO", __FUNCTION__, __LINE__);
		return;
	}	 
	PreservedBind(); 
	GLint location = glGetAttribLocation(program.id(), attrib_name.data());
	if (location == -1)
		global_log.Report<LogCode::FAULT>("Unable to locate shader attribute: " + attrib_name, __FUNCTION__, __LINE__);
	if (size == 0)
		return;
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, size, type, normalized, stride, (void*)offset);

	GLint err = glGetError(); 
	if (err != GL_NO_ERROR)
		global_log.Report<LogCode::FAULT>("Unable to setVertexAttribute: " + attrib_name + ", GL Error " + glGetErrorString(err), __FUNCTION__, __LINE__);

	PreservedUnbind();
}

void VertexArray::Bind() const{
	if (active_vertex_array == vao_)
		return;
	active_vertex_array = vao_;
	glBindVertexArray(vao_);
}

void VertexArray::Unbind() const{
	active_vertex_array = 0;
	glBindVertexArray(0);
}

void VertexArray::PreservedBind() {
	if (preserved_vao_ == vao_)
		return;
	preserved_vao_ = active_vertex_array;
	glBindVertexArray(vao_);
}

void VertexArray::PreservedUnbind() {
	if (preserved_vao_ == vao_)
		return;
	glBindVertexArray(preserved_vao_);
}

GLuint VertexArray::id() const {
	return vao_;
}

void VertexArray::Initialise() {
	if (vao_ == 0)
		glGenVertexArrays(1, &vao_);
}
