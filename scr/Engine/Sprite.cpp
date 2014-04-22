#include "Sprite.h"

namespace Pro{

	Sprite::Sprite()
	{
	}
	Sprite::Sprite(const std::string& name, SDL_Rect _rect)
	{
		setName(name);
		rect = _rect;
	}

	Sprite::~Sprite()
	{
	}


	void Sprite::attachSpriteSheet(SDL_Texture* _spriteSheet){
		spriteSheet = _spriteSheet;
	}

	SDL_Rect Sprite::getRect(){
		return rect;
	}
	void Sprite::setRect(SDL_Rect _rect){
		rect = _rect;
	}

	SDL_Texture* Sprite::getSpriteSheet(){
		return spriteSheet;
	}
}