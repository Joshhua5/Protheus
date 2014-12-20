#include "MeshObject.h"

using namespace Pro;

MeshObject::MeshObject(const string& name, unsigned start, unsigned size) : MeshObject() {
	this->name = name;
	this->size = size;
	this->start = start;
}

MeshObject::MeshObject() {
	name = "";
	start = 0;
	size = 0; 
	vertex_per_face = 0;
	floats_per_vertex = 0; 
	face_count = 0;
	face_format = FACE_FORMAT::UNDEFINED;

	temp = nullptr;
	tempWriter = nullptr;
}

MeshObject::~MeshObject() {
}

MeshObject::MeshObject(MeshObject&& rhs) {
	*this = rhs;
}

MeshObject& MeshObject::operator=(MeshObject&& rhs) {
	name = std::move(rhs.name);
	start = rhs.start;
	size = rhs.size; 
	vertex_per_face = rhs.vertex_per_face;
	floats_per_vertex = rhs.floats_per_vertex; 
	face_count = rhs.face_count;
	face_format = rhs.face_format;

	rhs.temp = nullptr;
	rhs.tempWriter = nullptr;
	return *this;
}
MeshObject::MeshObject(const MeshObject& rhs) {
	*this = rhs;
}
MeshObject& MeshObject::operator=(const MeshObject& rhs) {
	name = rhs.name;
	start = rhs.start;
	size = rhs.size; 
	vertex_per_face = rhs.vertex_per_face;
	floats_per_vertex = rhs.floats_per_vertex; 
	face_count = rhs.face_count;
	face_format = rhs.face_format;

	temp = rhs.temp;
	tempWriter = rhs.tempWriter;
	return *this;
}

bool     MeshObject::hasTexCoord() const {
	return has_tex_coord;
}

bool     MeshObject::hasNormals() const {
	return has_normals;
}

GLuint   MeshObject::vertexSize() const {
	return  floats_per_vertex;
}

GLuint   MeshObject::normalSize() const {
	if (!has_normals)
		return 0;
	return  3;
}

GLuint   MeshObject::texCoordSize() const {
	if (!has_tex_coord)
		return 0;
	return  tex_coord_per_vertex;
}

GLsizei  MeshObject::stride() const {
	if (has_normals && has_tex_coord)
		return tex_coord_per_vertex * sizeof(GLfloat) + floats_per_vertex * sizeof(GLfloat) + sizeof(GLfloat) * 3;
	else if (has_normals)
		return floats_per_vertex * sizeof(GLfloat) + sizeof(GLfloat) * 3;
	else if (has_tex_coord)
		return tex_coord_per_vertex * sizeof(GLfloat) + floats_per_vertex * sizeof(GLfloat);
	return floats_per_vertex * sizeof(GLfloat);
}

GLuint   MeshObject::normalOffset() const {
	if (!has_normals)
		return 0;
	if (has_tex_coord)
		return floats_per_vertex * sizeof(GLfloat) + tex_coord_per_vertex * sizeof(GLfloat);
	return floats_per_vertex * sizeof(GLfloat);
}

GLuint   MeshObject::vertexOffset() const {
	return 0;
}

GLuint   MeshObject::texCoordOffset() const {
	if (!has_tex_coord)
		return 0;
	return floats_per_vertex * sizeof(GLfloat);
}
GLenum   MeshObject::getMode() const {
	return vertex_per_face == 4 ? GL_QUADS : GL_TRIANGLE_STRIP;
}