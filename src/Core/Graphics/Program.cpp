#include "Program.h"

using namespace Pro;
using namespace Graphics;
using namespace Util;
using namespace Math;

static int active_program_id = 0;
static int preserved_id = 0;

Program::Program(bool initialize_gl) {

	if (initialize_gl)
		program_id = glCreateProgram();
	else
		program_id = 0;
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
	if (glIsShader(shader.getShader()))
		glAttachShader(program_id, shader.getShader());
	else
		has_error = true;
}


void Program::init() {
	if (program_id == 0)
		program_id = glCreateProgram();
}

GLuint Program::getID() const{
	return program_id;
}

void Program::use() {
	if (active_program_id == program_id)
		return;
	active_program_id = program_id;
	glUseProgram(program_id);
}

void Program::preservedUse() {
	preserved_id = active_program_id;
	if (program_id != active_program_id)
		glUseProgram(program_id);
}

void Program::preservedDisuse() {
	if (program_id != active_program_id)
		glUseProgram(preserved_id);
}

void Program::link() {
	glLinkProgram(program_id);
	GLint error_id;
	glGetProgramiv(program_id, GL_LINK_STATUS, &error_id);
	if (error_id == GL_FALSE) {
		GLint size;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &size);
		Buffer err(size);
		glGetProgramInfoLog(program_id, err.size(), nullptr, err.data<char>());
		error.reportErrorNR(err.data<char>());
		has_error = true;
	}
}

bool Program::hasError() const {
	return has_error;
}

inline GLint Program::getUniformLocation(GLuint program_id, const string& uniform_name) { 
	GLint location = 0;
	// Check if the location is already stored
	auto iterator = locations.find(uniform_name);
	if (iterator == locations.end()) {
		location = glGetUniformLocation(program_id, uniform_name.data());
		if (location == -1) {
			error.reportErrorNR("Unable to locate shader attribute: " + uniform_name);
			return -1;
		}
		locations.insert({ uniform_name, location });
	}
	else
		location = (*iterator).second;
	return location;
}
 

void Program::setUniform(const string& uniform_name, const Vector3<float>& value) {
	GLint location = getUniformLocation(program_id, uniform_name);
	if (location == -1)
		return;
	preservedUse();
	glUniform3f(location, value.x, value.y, value.z);
	preservedDisuse();
}

void Program::setUniform(const string& uniform_name, const Vector2<float>& value) {
	GLint location = getUniformLocation(program_id, uniform_name);
	if (location == -1)
		return;
	preservedUse();
	glUniform2f(location, value.x, value.y);
	preservedDisuse();
}

void Program::setUniform(const string& uniform_name, GLint value) {
	auto location = getUniformLocation(program_id, uniform_name.data());
	if (location == -1)
		return;
	preservedUse();
	glUniform1i(location, value);
	preservedDisuse();
}

void Program::setUniform(const string& uniform_name, const GLint* value, unsigned size) {
	GLint location = getUniformLocation(program_id, uniform_name);
	if (location == -1)
		return;
	preservedUse();
	glUniform1iv(location, size, value);
	preservedDisuse();
}

void Program::setUniform(const string& uniform_name, const Matrix44<float>& value) {
	GLint location = getUniformLocation(program_id, uniform_name);
	if (location == -1)
		return;
	preservedUse();
	glUniformMatrix4fv(location, 1, GL_FALSE, (const float*)value._m);
	preservedDisuse();
}

void Program::setUniform(const string& uniform_name, float* value, unsigned count) {
	GLint location = getUniformLocation(program_id, uniform_name);
	if (location == -1)
		return;
	preservedUse();
	glUniform1fv(location, count, value);
	preservedDisuse();
}

