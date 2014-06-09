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
#include <vector>
#include "..\graphics\Sprite.h"

namespace Pro{
	namespace Asset{
		class AnimatedSprite : 
			public Component::CGUID
		{
		private:
			std::vector<Sprite*> frames;
			unsigned short currentFrame;
		public:
			AnimatedSprite(const std::string& name);
			AnimatedSprite();
			~AnimatedSprite();

			void nextFrame();
			Sprite* getFrame();
			Sprite* getFrame(int);
			void addFrame(Sprite*);

			// LUA Functions 

			// returns the Metatable's name assosiated with this object
			static inline string lGetMetatable(){
				return "gameobject_animated_sprite_metatable";
			}
		}; 
	}
}