#include "Mesh.h"

using namespace Pro;

Mesh::Mesh(GLuint verticies, GLuint elements, GLenum mode, bool vertexContainsW, bool hasUV, bool hasNormals) {
	this->mode = mode;
	this->verticies = verticies;
	this->elements = elements;
	vertexW = vertexContainsW;
	this->hasNormals = hasNormals;
	this->hasUV = hasUV; 
}

Mesh::Mesh(Mesh&& rhs) {
	objects = std::move(rhs.objects);
	mode = rhs.mode;
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
		mode = rhs.mode;
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


vector<MeshObject>& Mesh::getObjects() {
	return objects;
}

GLenum Mesh::getMode() const {
	return mode;
}

void Mesh::bind(){
	glBindBuffer(GL_ARRAY_BUFFER, verticies);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements); 
}

void Mesh::unbind(){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}