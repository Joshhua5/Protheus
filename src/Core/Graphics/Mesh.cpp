#include "Mesh.h"

using namespace Pro;

Mesh::Mesh(GLuint verticies, GLuint elements) {
	this->verticies = verticies;
	this->elements = elements; 
}

Mesh::Mesh(Mesh&& rhs) {
	objects = std::move(rhs.objects); 
	verticies = rhs.verticies;
	elements = rhs.elements;
	object_count = rhs.object_count; 
	rhs.verticies = rhs.elements = 0;
}

Mesh& Mesh::operator=(Mesh&& rhs) {
	objects = std::move(rhs.objects); 
	verticies = rhs.verticies;
	elements = rhs.elements;
	object_count = rhs.object_count; 
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


void Mesh::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, verticies);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
}

void Mesh::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


