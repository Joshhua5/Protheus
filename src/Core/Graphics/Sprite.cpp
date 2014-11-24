#include "Sprite.h"

using namespace Pro;
using namespace Asset;

Sprite::Sprite(SDL_Texture* tex){
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

void load(const CBuffer&);