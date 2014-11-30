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
			GLFWwindow* window;
		public:
			Renderer(const Window& window); 
			~Renderer();
			  

			void setWindow(const Window& window);
			void startFrame();
			void endFrame(); 
		};
	}
}