#include "Sprite.h"

using namespace Pro;
using namespace Asset;

Sprite::Sprite(Texture* tex){
	m_texture = tex;
	center.x = (int) (dimensions.x / 2);
	center.y = (int) (dimensions.y / 2);
}

Sprite::~Sprite(){ 
	m_texture = nullptr;
}

Texture* Sprite::getTexture(){
	return m_texture;
}

Vector2& Sprite::getCenter(){
	return center;
} 

Vector3& Sprite::getPosition() {
	return position;
}

Vector2& Sprite::getDimensions() {
	return dimensions;
}

void Sprite::setPosition(const Vector3& pos) {
	position = pos;
}

void Sprite::setDimensions(const Vector2& dim) {
	dimensions = dim;
}