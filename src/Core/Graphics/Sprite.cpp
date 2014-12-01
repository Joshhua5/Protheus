#include "Sprite.h"

using namespace Pro;
using namespace Graphics;

Sprite::Sprite(Texture* tex){
	if (tex == nullptr)
		return;
	m_texture = tex;
	dimensions = m_texture->getDimensions();
	center.x = dimensions.x / 2;
	center.y = dimensions.y / 2;
}

Sprite::~Sprite(){ 
	m_texture = nullptr;
}

Texture* Sprite::getTexture(){
	return m_texture;
}

Vector2<float>& Sprite::getCenter(){
	return center;
} 

Vector3<float>& Sprite::getPosition() {
	return position;
}

Vector2<float>& Sprite::getDimensions() {
	return dimensions;
}

void Sprite::setPosition(const Vector3<float>& pos) {
	position = pos;
}

void Sprite::setDimensions(const Vector2<float>& dim) {
	dimensions = dim;
}