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