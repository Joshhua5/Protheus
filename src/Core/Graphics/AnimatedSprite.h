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
	namespace Graphics{
		class AnimatedSprite
		{
		private:
			std::vector<Sprite> frames;
			unsigned frame_count;
		public:
			AnimatedSprite();
			~AnimatedSprite(); 

			Sprite& getFrame(unsigned index);

			// Get the sprites to load from a file
			// or a spritesheet instead of
			// passing preloaded sprites
			// unless passed as r value
			void addFrame(Sprite&&);
			 
			unsigned getFrameCount() const; 
		};
	}
}