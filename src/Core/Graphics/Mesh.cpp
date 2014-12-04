#include "Mesh.h"

using namespace Pro;

Mesh::Mesh(GLuint verticies, GLuint elements, bool vertexContainsW, bool hasUV, bool hasNormals) {
	this->verticies = verticies;
	this->elements = elements;
	vertexW = vertexContainsW;
	this->hasNormals = hasNormals;
	this->hasUV = hasUV;
}

Mesh::Mesh(Mesh&& rhs) {

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
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &elements);
	glDeleteBuffers(1, &verticies);
}

void Mesh::attachObject(MeshObject&& model) {
	objects.push_back(model);
}

MeshObject::MeshObject(MeshObject&& rhs) {
	name = std::move(rhs.name);
	start = rhs.start;
	size = rhs.size;
	vba = rhs.vba;
	rhs.vba = 0;
}
MeshObject::MeshObject() {
	name = "";
	start = 0;
	size = 0;
	vba = 0;
}

MeshObject::~MeshObject() {
	glDeleteVertexArrays(1, &vba);
}