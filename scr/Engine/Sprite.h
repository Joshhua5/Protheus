#pragma once
#include <SDL.h>
#include <string>
 
#include "Math.h"
#include "CGUID.h"

namespace Pro{
	namespace Asset{

		class Sprite : 
			public CGUID
		{
		private:
			SDL_Texture* spriteSheet;
			Math::Vector4 rect; 
		public:
			Sprite::Sprite(const std::string& name, Math::Vector4 rect);
			Sprite();
			~Sprite();

			void attachSpriteSheet(SDL_Texture*);
			SDL_Texture* getSpriteSheet();
			Math::Vector4 getRect();
			void setRect(Math::Vector4);
		};
	}
}