#include "Sprite.h"

using namespace Pro;
using namespace Asset;

Sprite::Sprite(const string& name, Vector2& dimensions, SDL_Texture* tex) : CGUID(name), Area(dimensions){
	texture = tex;
	center.x = (int) (dimensions.x / 2);
	center.y = (int) (dimensions.y / 2);
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(texture);
}

SDL_Texture* Sprite::getTexture(){
	return texture;
}

SDL_Point* Sprite::getCenter(){
	return &center;
}

void Sprite::releaseTexture(){
	SDL_DestroyTexture(texture);
}