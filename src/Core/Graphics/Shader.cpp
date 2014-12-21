#include "Shader.h"

using namespace Pro;
  
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
		error.reportError("Unable to load shader: " + string(error_log.data<char>()));
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
		error.reportError("Unable to load shader: " + string(error_log.data<char>()));
	}
}

Shader::Shader() {
	m_shader_id = 0;
	m_shader_type = GL_INVALID_ENUM;
}

Shader::Shader(string shader, GLenum shader_type) {
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
		error.reportError("Unable to load shader: " + string(error_log.data<char>()));
	}
}

Shader::~Shader()
{
	glDeleteShader(m_shader_id);
}


GLuint Shader::getShader() const {
	return m_shader_id;
}

bool Shader::init(const Buffer& shader, GLenum shader_type) {
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
		error.reportError("Unable to load shader: " + m_shader_id);
		return false;
	}

	return true;
}
bool Shader::init(const string& shader, GLenum shader_type) {
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
		error.reportError("Unable to load shader: " + m_shader_id);
		return false;
	}

	return true;
}