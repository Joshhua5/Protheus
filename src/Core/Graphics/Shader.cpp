#include "Shader.h"

using namespace Pro;

Shader::Shader(const CBuffer& shader, GLenum shader_type)
{
	m_shader_id = glCreateShader(shader_type);
	 
	GLint size = shader.size();
	auto data = shader.data<char*>();
	glShaderSource(m_shader_id,
		1,
		(const GLchar**) data,
		(const GLint*) &size);

	glCompileShader(m_shader_id);

	glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &size);
	if (size == GL_FALSE)
		error.reportError("Unable to load shader: " + m_shader_id);

}

Shader::Shader(const char* shader, GLenum shader_type) {
	m_shader_id = glCreateShader(shader_type);

	GLint size = sizeof(*shader);
	glShaderSource(m_shader_id,
		1,
		(const GLchar**) shader,
		(const GLint*) &size);

	glCompileShader(m_shader_id);

	glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &size);
	if (size == GL_FALSE) 
		error.reportError("Unable to load shader: " + m_shader_id); 
}



Shader::~Shader()
{
	glDeleteShader(m_shader_id); 
}


GLuint Shader::getShader() const {
	return m_shader_id;
}

bool Shader::init(const CBuffer& shader, GLenum shader_type) {
	m_shader_id = glCreateShader(shader_type);

	GLint size = shader.size();
	auto data = shader.data<char*>();
	glShaderSource(m_shader_id,
		1,
		(const GLchar**) data,
		(const GLint*) &size);

	glCompileShader(m_shader_id);

	glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &size);
	if (size == GL_FALSE) {
		error.reportError("Unable to load shader: " + m_shader_id);
		return false;
	}

	return true;
}
bool Shader::init(const char* shader, GLenum shader_type) {
	m_shader_id = glCreateShader(shader_type);

	GLint size = sizeof(*shader); 
	glShaderSource(m_shader_id,
		1,
		(const GLchar**) shader,
		(const GLint*) &size);

	glCompileShader(m_shader_id);

	glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &size);
	if (size == GL_FALSE) {
		error.reportError("Unable to load shader: " + m_shader_id);
		return false;
	}

	return true;
}