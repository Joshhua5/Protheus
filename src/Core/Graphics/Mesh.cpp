#include "Mesh.h"

using namespace Pro;

Mesh::Mesh(GLuint verticies, GLuint elements, GLenum format, bool vertexContainsW, bool hasUV, bool hasNormals) {
	this->format = format;
	this->verticies = verticies;
	this->elements = elements;
	vertexW = vertexContainsW;
	this->hasNormals = hasNormals;
	this->hasUV = hasUV; 
}

Mesh::Mesh(Mesh&& rhs) {
	objects = std::move(rhs.objects);
	verticies = rhs.verticies;
	elements = rhs.elements;
	object_count = rhs.object_count;
	vertexW = rhs.vertexW;
	hasUV = rhs.hasUV;
	hasNormals = rhs.hasNormals;
	rhs.verticies = rhs.elements = 0;
}

Mesh& Mesh::operator=(Mesh&& rhs) { 
	objects = std::move(rhs.objects);
	verticies = rhs.verticies;
	elements = rhs.elements;
	object_count = rhs.object_count;
	vertexW = rhs.vertexW;
	hasUV = rhs.hasUV;
	hasNormals = rhs.hasNormals; 
	rhs.verticies = rhs.elements = 0; 
	return *this;
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &elements);
	glDeleteBuffers(1, &verticies);
}

void Mesh::attachObject(MeshObject&& model) {
	objects.push_back(model);
}

MeshObject* Mesh::getObject(const string& object_name) { 
	for (unsigned x = 0; x < objects.size(); ++x)
		if (objects[x].name == object_name)
			return &objects[x];
	error.reportErrorNR("Unable to find mesh object" + object_name);  
	return nullptr;
}

void Mesh::bindProgram(const string& object_name, GLuint program_id) { 
	for (unsigned x = 0; x < objects.size(); ++x) {
		auto& obj = objects[x];
		if (obj.name == object_name)
			obj.program = program_id;
	}
}
void Mesh::bindProgram(GLuint program_id) {
	for (unsigned x = 0; x < objects.size(); ++x)
		objects[x].program = program_id;
}
 
void Mesh::bind(){
	glBindBuffer(GL_ARRAY_BUFFER, verticies);
	vao.bind();
}

void Mesh::unbind(){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	vao.unbind();
}

void Mesh::draw(){
	bind(); 
	for (const auto& obj : objects) {
		// Find a way to use the Object's VBA's 
		obj.setUniforms();
		glDrawElements(format, obj.size, GL_UNSIGNED_INT, (const void*)obj.start);
	} 
	unbind();
}


inline void  Mesh::setVertexAttribute(const string& attrib_name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const unsigned offset) {
	for (const auto& obj : objects)
		obj.setVertexAttribute(attrib_name, size, type, normalized, stride, offset); 
}