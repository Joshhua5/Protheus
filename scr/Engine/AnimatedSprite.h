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
#include "Sprite.h"

namespace Pro{
	namespace Asset{
		class AnimatedSprite
		{
		private:
			std::vector<Sprite*> frames;
			unsigned short currentFrame;
		public:
			AnimatedSprite();
			virtual ~AnimatedSprite();

			void nextFrame();
			Sprite* getFrame();
			Sprite* getFrame(int);
			void addFrame(Sprite*);

			// LUA Functions 

		}; 
	}
}