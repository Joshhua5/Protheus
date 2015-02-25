#include "Shader.h"

using namespace Pro;
using namespace Graphics;
using namespace Util;
  
Shader::Shader(const Buffer& shader, GLenum shader_type) {
	m_shader_id = glCreateShader(shader_type);

	GLint size = shader.size();
	const GLchar* data = shader.data<GLchar>();

	glShaderSource(m_shader_id,
		1,
		&data,
		&size);
	glCompileShader(m_shader_id);

	glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &size);
	if (size == GL_FALSE) {
		Buffer error_log(512);
		glGetShaderInfoLog(m_shader_id, 512, NULL, error_log.data<GLchar>());
		global_log.Report<LogCode::ERROR>("Unable to load shader: " + std::string(error_log.data<char>()), __FUNCTION__, __LINE__);
	}
}
 
Shader::Shader(Buffer&& shader, GLenum shader_type) {
	m_shader_id = glCreateShader(shader_type);

	GLint size = shader.size();
	const GLchar* data = shader.data<GLchar>();

	glShaderSource(m_shader_id,
		1,
		&data,
		&size);
	glCompileShader(m_shader_id);

	glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &size);
	if (size == GL_FALSE) {
		Buffer error_log(512);
		glGetShaderInfoLog(m_shader_id, 512, NULL, error_log.data<GLchar>());
		global_log.Report<LogCode::ERROR>("Unable to load shader: " + std::string(error_log.data<char>()), __FUNCTION__, __LINE__);
	}
}

Shader::Shader() {
	m_shader_id = 0;
	m_shader_type = GL_INVALID_ENUM;
}

Shader::Shader(const std::string& shader, GLenum shader_type) {
	m_shader_id = glCreateShader(shader_type);

	GLint size = shader.length();
	const GLchar* data = shader.data();

	glShaderSource(m_shader_id,
		1,
		&data,
		&size);

	glCompileShader(m_shader_id);

	glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &size);
	if (size == GL_FALSE) {
		Buffer error_log(512);
		glGetShaderInfoLog(m_shader_id, 512, NULL, error_log.data<GLchar>());
		global_log.Report<LogCode::ERROR>("Unable to load shader: " + std::string(error_log.data<char>()), __FUNCTION__, __LINE__);
	}
}

Shader::~Shader()
{
	glDeleteShader(m_shader_id);
}


GLuint Shader::GetShader() const {
	return m_shader_id;
}

bool Shader::Init(const Buffer& shader, GLenum shader_type) {
	m_shader_id = glCreateShader(shader_type);

	GLint size = shader.size();
	auto data = shader.data<char*>();
	glShaderSource(m_shader_id,
		1,
		(const GLchar**)data,
		(const GLint*)&size);

	glCompileShader(m_shader_id);

	glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &size);
	if (size == GL_FALSE) {
		global_log.Report<LogCode::ERROR>("Unable to load shader: " + m_shader_id, __FUNCTION__, __LINE__);
		return false;
	}

	return true;
}
bool Shader::Init(const std::string& shader, GLenum shader_type) {
	if (m_shader_id != 0)
		return false;
	m_shader_id = glCreateShader(shader_type);

	const char* data = shader.data();
	const GLint size = shader.size();

	glShaderSource(m_shader_id, 1,
		&data,
		&size);

	glCompileShader(m_shader_id);

	GLint glError = 0;
	glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &glError);
	if (glError == GL_FALSE) {
		global_log.Report<LogCode::ERROR>("Unable to load shader: " + m_shader_id, __FUNCTION__, __LINE__);
		return false;
	}

	return true;
}