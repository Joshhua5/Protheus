#include "Mesh.h"

using namespace Pro;

Mesh::Mesh(GLuint verticies, GLuint elements, GLenum mode, bool vertexContainsW, bool tex_coord_w, bool hasUV, bool hasNormals) {
	this->mode = mode;
	this->verticies = verticies;
	this->elements = elements;
	this->tex_coord_w = tex_coord_w;
	vertex_w = vertexContainsW;
	has_normals = hasNormals;
	has_tex_coord = hasUV;
}

Mesh::Mesh(Mesh&& rhs) {
	objects = std::move(rhs.objects);
	mode = rhs.mode;
	verticies = rhs.verticies;
	elements = rhs.elements;
	object_count = rhs.object_count;
	tex_coord_w = rhs.tex_coord_w;
	vertex_w = rhs.vertex_w;
	has_tex_coord = rhs.has_tex_coord;
	has_normals = rhs.has_normals;
	rhs.verticies = rhs.elements = 0;
}

Mesh& Mesh::operator=(Mesh&& rhs) {
	objects = std::move(rhs.objects);
	mode = rhs.mode;
	verticies = rhs.verticies;
	elements = rhs.elements;
	object_count = rhs.object_count;
	tex_coord_w = rhs.tex_coord_w;
	vertex_w = rhs.vertex_w;
	has_tex_coord = rhs.has_tex_coord;
	has_normals = rhs.has_normals;
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

void Mesh::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, verticies);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
}

void Mesh::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool Mesh::hasTexCoord() const {
	return has_tex_coord;
}

bool Mesh::hasNormals() const {
	return has_normals;
}

GLuint  Mesh::vertexSize() const {  
	return  (vertex_w ? 4 : 3);
}

GLuint  Mesh::normalSize() const {
	if (!has_normals)
		return 0;
	return  3;
}

GLuint  Mesh::texCoordSize() const {
	if (!has_tex_coord)
		return 0;
	return  (tex_coord_w ? 3 : 2);
}

GLsizei  Mesh::stride() const {
	if (has_normals && has_tex_coord)
		return 
		sizeof(GLfloat) * (tex_coord_w ? 3 : 2) +
		sizeof(GLfloat) * (vertex_w ? 4 : 3) +
		sizeof(GLfloat) * 3;
	else if (has_normals)
		return 
		sizeof(GLfloat) * (vertex_w ? 4 : 3) +
		sizeof(GLfloat) * 3;
	else if (has_tex_coord)
		return
		sizeof(GLfloat) * (tex_coord_w ? 3 : 2) +
		sizeof(GLfloat) * (vertex_w ? 4 : 3);
	return sizeof(GLfloat) * (vertex_w ? 4 : 3);
}

GLuint  Mesh::normalOffset() const {
	if (!has_normals)
		return 0;
	if (has_tex_coord)
		return sizeof(GLfloat) * (vertex_w ? 4 : 3) + sizeof(GLfloat) * 3;
	return sizeof(GLfloat) * (vertex_w ? 4 : 3);
}

GLuint  Mesh::vertexOffset() const {
	return 0;
}

GLuint  Mesh::texCoordOffset() const {
	if (!has_tex_coord)
		return 0;
	return sizeof(GLfloat) * (vertex_w ? 4 : 3);
}