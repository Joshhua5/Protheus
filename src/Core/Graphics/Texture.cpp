#include "Texture.h"

using namespace Pro;

Texture::Texture(const GLuint _texture_id,const Vector2<unsigned>& dim)
{
	texture_id = _texture_id;
	dimensions = dim;
}


Texture::~Texture()
{
	glDeleteTextures(1, &texture_id);
}


Texture::Texture(Texture&& other) { 
	this->texture_id = other.texture_id;
	this->dimensions = other.dimensions; 
	other.texture_id = 0;
}

Texture& Texture::operator=(Texture&& rhs) {
	this->texture_id = rhs.texture_id;
	this->dimensions = rhs.dimensions;
	rhs.texture_id = 0;
	return *this;
}


const GLuint Texture::getTextureID() const {
	return texture_id;
}


Vector2<unsigned>& Texture::getDimensions() {
	return dimensions;
}

void Texture::setFilter(GLenum type) {
	glBindTexture(GL_TEXTURE_2D, texture_id); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, type);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setBorder(GLenum type) {
	// CONSIDER
	// checking the passed in values to make sure they're valid
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, type);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::bind() const{
	glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}