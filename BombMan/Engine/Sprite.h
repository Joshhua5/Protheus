#pragma once
#include "Name.h"
#include <SDL.h>
#include <string>
namespace Pro{

	class Sprite :
		public Name
	{
	private:
		SDL_Texture* spriteSheet;
		SDL_Rect rect;

	public:
		Sprite::Sprite(const std::string& name, SDL_Rect rect);
		Sprite();
		~Sprite();

		void attachSpriteSheet(SDL_Texture*);
		SDL_Texture* getSpriteSheet();
		SDL_Rect getRect();
		void setRect(SDL_Rect);
	};

}