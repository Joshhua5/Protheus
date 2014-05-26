#include "Sprite.h"
using namespace Pro;
using namespace Asset;

Sprite::Sprite()
{
}
Sprite::Sprite(const std::string& name, Math::Vector4& _rect) : 
CGUID(name)
{ 
	rect = _rect;
}

Sprite::~Sprite()
{
}

void Sprite::attachSpriteSheet(SDL_Texture* _spriteSheet){
	spriteSheet = _spriteSheet;
}

Math::Vector4 Sprite::getRect(){
	return rect;
}
void Sprite::setRect(Math::Vector4& _rect){
	rect = _rect;
}

SDL_Texture* Sprite::getSpriteSheet(){
	return spriteSheet;
}
