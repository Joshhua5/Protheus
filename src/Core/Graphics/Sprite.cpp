#include "Sprite.h"

using namespace Pro;
using namespace Graphics;

Sprite::Sprite(Texture* tex){
	if (tex == nullptr)
		return;
	texture_ = tex;
	dimensions_ = texture_->dimensions();
	center_.x = dimensions_.x / 2;
	center_.y = dimensions_.y / 2;
}

Sprite::~Sprite(){ 
	texture_ = nullptr;
}

Texture* Sprite::texture(){
	return texture_;
}

Vector2<float>& Sprite::center(){
	return center_;
} 

Vector3<float>& Sprite::position() {
	return position_;
}

Vector2<float>& Sprite::dimensions() {
	return dimensions_;
}

void Sprite::position(const Vector3<float>& pos) {
	position_ = pos;
}

void Sprite::dimensions(const Vector2<float>& dim) {
	dimensions_ = dim;
}