#include "Shader.h"

using namespace Pro;
using namespace Graphics;
using namespace Util;
 
Shader::Shader() {
	m_shader_id = 0;
	m_shader_type = GL_INVALID_ENUM;
}

Shader::Shader(const std::string& shader, GLenum shader_type) : Shader() {
	Init(shader, shader_type);
}

Shader::Shader(const Buffer& shader, GLenum shader_type) : Shader() { 
	Init(shader, shader_type);
}

Shader::~Shader()
{
	if (m_shader_id != 0)
		glDeleteShader(m_shader_id);
}

GLuint Shader::GetShader() const {
	return m_shader_id;
}

bool Shader::Validate() const{
    GLint status;
    glGetIntegerv(GL_VALIDATE_STATUS, &status);
    glValidateProgram(m_shader_id);
    return status == GL_TRUE;
}

bool Shader::Init(const Buffer& shader, GLenum shader_type) {
	if (m_shader_id != 0) {
		global_log.Report<LogCode::FAULT>("Shader already initialised", __FUNCTION__, __LINE__);
		return false;
	}
	if (shader.size() == 0) {
		global_log.Report<LogCode::FAULT>("Null shader passed, did the file load correctly?", __FUNCTION__, __LINE__);
		return false;
	}
	m_shader_id = glCreateShader(shader_type);

	GLint size = (GLint)shader.size();
	const GLchar* data = shader.data<char>();

	glShaderSource(m_shader_id,
		1,
		(const GLchar**)&data,
		(const GLint*)&size);

	glCompileShader(m_shader_id);

	GLint shader_error; 
	GLenum gl_error = glGetError();
	glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &shader_error);
	if (shader_error == GL_FALSE || gl_error != GL_NO_ERROR) {
		GLsizei log_size;
		glGetShaderiv(m_shader_id, GL_INFO_LOG_LENGTH, &log_size);
		Buffer error_log(log_size);
		if (log_size != 0) {
			glGetShaderInfoLog(m_shader_id, log_size, NULL, error_log.data<GLchar>());
			global_log.Report<LogCode::FAULT>("Unable to compile/link shader " + std::to_string(m_shader_id) +
				" OpenGL Error: " + glGetErrorString(gl_error) +
				"\n" + error_log.data<char>(),
				__FUNCTION__, __LINE__);
		}
		else {
			global_log.Report<LogCode::FAULT>("Unable to compile/link shader " + std::to_string(m_shader_id) +
				" OpenGL Error: " + glGetErrorString(gl_error) +
				"\n NO error log avaliable",
				__FUNCTION__, __LINE__);
		}
		glDeleteShader(m_shader_id);
		m_shader_id = 0;
		return false;
	}
	return true;
}
bool Shader::Init(const std::string& shader, GLenum shader_type) {
	return Init(Buffer((void*)shader.data(), (int)shader.size(), false), shader_type);
}