

#include "Sprite.h"

using namespace Pro;
using namespace Asset;

Sprite::Sprite(const string& name, Vector2& dimensions, SDL_Texture* tex) : CGUID(name) , Area(dimensions){
	texture = tex;
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(texture);
}
  
SDL_Texture* Sprite::getTexture(){
	return texture;
}