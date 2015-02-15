#include "MeshObject.h"

using namespace Pro;
using namespace Graphics;


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
	vertex_count = 0;
	face_format = FACE_FORMAT::UNDEFINED;

	temp = nullptr;
	tempWriter = nullptr;
}

MeshObject::~MeshObject() {
}

MeshObject::MeshObject(MeshObject&& rhs) {
	*this = rhs;
}

inline void copy(MeshObject* ptr, const MeshObject& rhs){ 
	ptr->start = rhs.start;
	ptr->size = rhs.size; 
	ptr->vertex_per_face = rhs.vertex_per_face;
	ptr->tex_coord_per_vertex = rhs.tex_coord_per_vertex;
	ptr->floats_per_vertex = rhs.floats_per_vertex; 
	ptr->vertex_count = rhs.vertex_count;
	ptr->face_format = rhs.face_format;
	ptr->has_tex_coord = rhs.has_tex_coord;
	ptr->has_normals = rhs.has_normals;
}

MeshObject& MeshObject::operator=(MeshObject&& rhs) {
	name = std::move(rhs.name);
	copy(this, rhs);
	rhs.temp = nullptr;
	rhs.tempWriter = nullptr;
	return *this;
}
MeshObject::MeshObject(const MeshObject& rhs) {
	*this = rhs;
}
MeshObject& MeshObject::operator=(const MeshObject& rhs) {
	name = rhs.name; 

	copy(this, rhs); 

	temp = rhs.temp;
	tempWriter = rhs.tempWriter;
	return *this;
}

bool MeshObject::HasTexCoord() const {
	return has_tex_coord ? 1 : 0;
}

bool MeshObject::HasNormals() const {
	return has_normals ? 1 : 0;
}

GLuint   MeshObject::VertexSize() const {
	return  floats_per_vertex;
}

GLuint   MeshObject::NormalSize() const {
	if (!has_normals)
		return 0;
	return  3;
}

GLuint   MeshObject::TexCoordSize() const {
	if (!has_tex_coord)
		return 0;
	return  tex_coord_per_vertex;
}

GLsizei  MeshObject::Stride() const {
	if (has_normals && has_tex_coord)
		return tex_coord_per_vertex * sizeof(GLfloat) + floats_per_vertex * sizeof(GLfloat) + sizeof(GLfloat) * 3;
	else if (has_normals)
		return floats_per_vertex * sizeof(GLfloat) + sizeof(GLfloat) * 3;
	else if (has_tex_coord)
		return tex_coord_per_vertex * sizeof(GLfloat) + floats_per_vertex * sizeof(GLfloat);
	return floats_per_vertex * sizeof(GLfloat);
}

GLuint   MeshObject::NormalOffset() const {
	if (!has_normals)
		return 0;
	if (has_tex_coord)
		return floats_per_vertex * sizeof(GLfloat) + tex_coord_per_vertex * sizeof(GLfloat);
	return floats_per_vertex * sizeof(GLfloat);
}

GLuint   MeshObject::VertexOffset() const {
	return 0;
}

GLuint   MeshObject::TexCoordOffset() const {
	if (!has_tex_coord)
		return 0;
	return floats_per_vertex * sizeof(GLfloat);
}
GLenum   MeshObject::GetMode() const {
	return GL_TRIANGLES;
}