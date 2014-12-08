#include "MeshObject.h"

using namespace Pro;

MeshObject::MeshObject(MeshObject&& rhs) {
	name = std::move(rhs.name);
	start = rhs.start;
	size = rhs.size;
}
MeshObject::MeshObject() {
	name = "";
	start = 0;
	size = 0;
}

MeshObject::~MeshObject() {
}

void MeshObject::setUniforms() const { 
	for (const auto& uni : uniforms1u){	glUniform1i(uni.first, uni.second); } 
	for (const auto& uni : uniforms3f){ glUniform3f(uni.first, uni.second.x, uni.second.y, uni.second.z); }
		
}

void MeshObject::setVertexAttribute(const string& attrib_name, GLint size, GLenum type,
	GLboolean normalized, GLsizei stride, const unsigned offset) const {
	Program::setVertexAttribute(program, attrib_name, size, type, normalized, stride, offset);
}

void MeshObject::setUniform(const string& uniform_name, const Vector3<float>& value){
	auto location = glGetUniformLocation(program, uniform_name.data());
	if (location == -1)
		return error.reportErrorNR("Unable to locate shader attribute: " + uniform_name);
	uniforms3f.push_back({ location, value });
}

void MeshObject::setUniform(const string& uniform_name, GLint value){ 
	auto location = glGetUniformLocation(program, uniform_name.data());
	if (location == -1)
		return error.reportErrorNR("Unable to locate shader attribute: " + uniform_name); 
	uniforms1u.push_back({ location, value });
}

void MeshObject::setModelMatrix(const Matrix44<float>& matrix){
	model_matrix = matrix;
}

