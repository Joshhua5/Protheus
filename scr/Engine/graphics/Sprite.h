/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once
#include <SDL.h>
#include <string>
 
#include "Math.h"
#include "..\component\CGUID.h"

namespace Pro{
	namespace Asset{

		class Sprite : 
			public Component::CGUID
		{
		private:
			SDL_Texture* spriteSheet;
			Math::Vector4 rect; 
		public:
			Sprite::Sprite(const std::string& name, Math::Vector4& rect);
			Sprite();
			~Sprite();

			void attachSpriteSheet(SDL_Texture*);
			SDL_Texture* getSpriteSheet();
			Math::Vector4 getRect();
			void setRect(Math::Vector4&);


			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "asset_sprite_metatable";
			}
		};
	}
}