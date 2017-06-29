/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <vector>
#include "Texture.h"

namespace Pro{
	namespace Graphics{
		//! Depreciated
		class AnimatedSprite
		{
		private:
            std::vector<std::shared_ptr<Texture>> frames_;
		public: 

            std::shared_ptr<Texture> GetFrame(const unsigned index);
			 
			void addFrame(std::shared_ptr<Texture> texture);

			unsigned getFrameCount() const;
		};
	}
}
