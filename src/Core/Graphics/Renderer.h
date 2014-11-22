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

#include <unordered_map> 
#include "SpriteBatcher.h"
#include "Window.h"

namespace Pro{
	namespace Graphics{
		class Renderer
		{
		private: 
			SDL_Renderer* renderer; 
		public:
			Renderer(const Window& window);
			~Renderer();

			SDL_Renderer* getRenderer(); 
			  
			void startFrame();
			void endFrame(); 
		};
	}
}