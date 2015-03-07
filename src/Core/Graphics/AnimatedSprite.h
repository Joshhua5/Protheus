/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <vector>
#include <smart_ptr.h>
#include "Texture.h"

namespace Pro{
	namespace Graphics{
		class AnimatedSprite
		{
		private:
			std::vector<Util::smart_ptr<Texture>> frames_;
			unsigned frame_count_;
		public: 

			Util::smart_ptr<Texture> GetFrame(const unsigned index);
			 
			void addFrame(Util::smart_ptr<Texture> texture);

			unsigned getFrameCount() const;
		};
	}
}
