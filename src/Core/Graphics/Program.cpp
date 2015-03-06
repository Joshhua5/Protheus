#include "Program.h"

using namespace Pro;
using namespace Graphics;
using namespace Util;
using namespace Math;
using namespace std;

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

void Program::AttachShader(const Shader& shader) {
	if (glIsShader(shader.GetShader()))
		glAttachShader(program_id, shader.GetShader());
	else
		has_error = true;
}


void Program::Init() {
	if (program_id == 0)
		program_id = glCreateProgram();
}

GLuint Program::id() const{
	return program_id;
}

void Program::Use() {
	if (active_program_id == program_id)
		return;
	active_program_id = program_id;
	glUseProgram(program_id);
}

void Program::PreservedUse() {
	preserved_id = active_program_id;
	if (program_id != active_program_id)
		glUseProgram(program_id);
}

void Program::PreservedDisuse() {
	if (program_id != active_program_id)
		glUseProgram(preserved_id);
}

void Program::Link() {
	glLinkProgram(program_id);
	GLint error_id;
	glGetProgramiv(program_id, GL_LINK_STATUS, &error_id);
	if (error_id == GL_FALSE) {
		GLint size;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &size);
		Buffer err(size);
		glGetProgramInfoLog(program_id, err.size(), nullptr, err.data<char>());
		global_log.Report<LogCode::ERROR>(err.data<char>(), __FUNCTION__, __LINE__);
		has_error = true;
	}
}

bool Program::HasError() const {
	return has_error;
}

inline GLint Program::getUniformLocation(GLuint program_id, const std::string& uniform_name) { 
	GLint location = 0;
	// Check if the location is already stored
	auto iterator = locations.find(uniform_name);
	if (iterator == locations.end()) {
		location = glGetUniformLocation(program_id, uniform_name.data());
		if (location == -1) {
			global_log.Report<LogCode::ERROR>("Unable to locate shader attribute: " + uniform_name, __FUNCTION__, __LINE__);
			return -1;
		}
		locations.insert({ uniform_name, location });
	}
	else
		location = (*iterator).second;
	return location;
}
 

void Program::SetUniform(const string& uniform_name, const Vector3<float>& value) {
	GLint location = getUniformLocation(program_id, uniform_name);
	if (location == -1)
		return;
	PreservedUse();
	glUniform3f(location, value.x, value.y, value.z);
	PreservedDisuse();
}

void Program::SetUniform(const string& uniform_name, const Vector2<float>& value) {
	GLint location = getUniformLocation(program_id, uniform_name);
	if (location == -1)
		return;
	PreservedUse();
	glUniform2f(location, value.x, value.y);
	PreservedDisuse();
}

void Program::SetUniform(const string& uniform_name, GLint value) {
	auto location = getUniformLocation(program_id, uniform_name.data());
	if (location == -1)
		return;
	PreservedUse();
	glUniform1i(location, value);
	PreservedDisuse();
}

void Program::SetUniform(const string& uniform_name, const GLint* value, unsigned size) {
	GLint location = getUniformLocation(program_id, uniform_name);
	if (location == -1)
		return;
	PreservedUse();
	glUniform1iv(location, size, value);
	PreservedDisuse();
}

void Program::SetUniform(const string& uniform_name, const Matrix44<float>& value) {
	GLint location = getUniformLocation(program_id, uniform_name);
	if (location == -1)
		return;
	PreservedUse();
	glUniformMatrix4fv(location, 1, GL_FALSE, value.data());
	PreservedDisuse();
}

void Program::SetUniform(const string& uniform_name, float* value, unsigned count) {
	GLint location = getUniformLocation(program_id, uniform_name);
	if (location == -1)
		return;
	PreservedUse();
	glUniform1fv(location, count, value);
	PreservedDisuse();
}

