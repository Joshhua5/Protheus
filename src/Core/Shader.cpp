#include "Shader.h"

using namespace Pro;

Shader::Shader(const CBuffer& shader, GLenum shader_type)
{
	m_shader_id = glCreateShader(shader_type);
	 
	GLint size = shader.size();
	auto data = shader.data<char*>();
	glShaderSource(m_shader_id,
		1,
		data,
		&size);

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