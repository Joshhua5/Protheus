#include "Program.h"

using namespace Pro;

Program::Program() {
	program_id = glCreateProgram();
}


Program::~Program() {
	glDeleteProgram(program_id);
}

Program::Program(Program&& rhs) {
	program_id = rhs.program_id;
	rhs.program_id = 0;
}

Program& Program::operator=(Program&& rhs) {
	program_id = rhs.program_id;
	rhs.program_id = 0;
	return *this;
}

void Program::attachShader(const Shader& shader) {
	glAttachShader(program_id, shader.getShader());  
}


GLuint Program::getID() {
	return program_id;
}

void Program::setActive() {
	glUseProgram(program_id);
}

void Program::link() {
	glLinkProgram(program_id);
	GLint error_id;
	glGetProgramiv(program_id, GL_LINK_STATUS, &error_id);
	if (error_id == GL_FALSE) {
		GLint size;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &size);
		CBuffer err(size);
		glGetProgramInfoLog(program_id, err.size(), nullptr, err.data<char>());
		error.reportErrorNR(err.data<char>());
	}
} 

void Program::setVertexAttribute(const string& attrib_name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const unsigned offset) {
	setVertexAttribute(program_id, attrib_name, size, type, normalized, stride, offset);
}

void Program::setUniform(const string& uniform_name, const Vector3<float>& value) {

	setUniform(program_id, uniform_name, value);
}

void Program::setUniform(const string& uniform_name, GLint value) {

	setUniform(program_id, uniform_name, value);
}

void Program::setUniform(const string& uniform_name, const Matrix44<float>& value) {
	setUniform(program_id, uniform_name, value);
} 

void Program::setVertexAttribute(GLuint program_id, const string& attrib_name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const unsigned offset) {
	GLint location = glGetAttribLocation(program_id, attrib_name.data());
	if (location == -1)
		return error.reportErrorNR("Unable to locate shader attribute: " + attrib_name);
	glEnableVertexAttribArray(location);
	if (size == 0) 
		return;
	glVertexAttribPointer(location, size, type, normalized, stride, (void*) offset);
}

void Program::setUniform(GLuint program_id, const string& uniform_name, const Vector3<float>& value){
	auto location = glGetUniformLocation(program_id, uniform_name.data());
	if (location == -1)
		return error.reportErrorNR("Unable to locate shader attribute: " + uniform_name);
	glUniform3f(location, value.x, value.y, value.z);
}

void Program::setUniform(GLuint program_id, const string& uniform_name, GLint value){
	auto location = glGetUniformLocation(program_id, uniform_name.data());
	if (location == -1)
		return error.reportErrorNR("Unable to locate shader attribute: " + uniform_name);
	glUniform1i(location, value);
}

void Program::setUniform(GLuint program_id, const string& uniform_name, const Matrix44<float>& value){
	auto location = glGetUniformLocation(program_id, uniform_name.data());
	if (location == -1)
		return error.reportErrorNR("Unable to locate shader attribute: " + uniform_name);
	glUniformMatrix4fv(location, 1, GL_FALSE, (const float*) value._m);
} 


void Program::setUniform(GLuint program_id, const string& uniform_name, float* value, unsigned count) {
	auto location = glGetUniformLocation(program_id, uniform_name.data());
	if (location == -1)
		return error.reportErrorNR("Unable to locate shader attribute: " + uniform_name);
	glUniform1fv(location, count, value);
}