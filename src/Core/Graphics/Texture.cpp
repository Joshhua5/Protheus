#include "Texture.h"

using namespace Pro;
using namespace Graphics;

Texture::Texture(const Vector2<unsigned>& dim, GLenum format, GLenum type){
    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glTexImage2D(GL_TEXTURE_2D, 0, format, dim.x, dim.y, 0, format, type, 0);
    dimensions_ = dim;
}

Texture::Texture(const GLuint _texture_id,const Vector2<unsigned>& dim)
{
	texture_id_ = _texture_id;
	dimensions_ = dim;
}


Texture::~Texture()
{
	glDeleteTextures(1, &texture_id_);
}


Texture::Texture(Texture&& other) { 
	this->texture_id_ = other.texture_id_;
	this->dimensions_ = other.dimensions_; 
	other.texture_id_ = 0;
}

Texture& Texture::operator=(Texture&& rhs) {
	this->texture_id_ = rhs.texture_id_;
	this->dimensions_ = rhs.dimensions_;
	rhs.texture_id_ = 0;
	return *this;
}


const GLuint Texture::textureID() const {
	return texture_id_;
}


Vector2<unsigned>& Texture::dimensions() {
	return dimensions_;
}

void Texture::SetFilter(GLenum type) {
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, type);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetBorder(GLenum type) {
	// CONSIDER
	// checking the passed in values to make sure they're valid
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, type);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::Bind() const{	
	glBindTexture(GL_TEXTURE_2D, texture_id_);
}

void Texture::Unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}