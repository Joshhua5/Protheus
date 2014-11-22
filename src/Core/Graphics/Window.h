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
#include <Error.h>

namespace Pro {
	namespace Graphics {
		class Window
		{
			SDL_Window* window;
			std::string windowTitle;
			SDL_Rect dim;

		public:
			Window(const std::string& name);
			~Window();

			void setTitle(const std::string& title);
			void setWidth(unsigned int);
			void setHeight(unsigned int);
			void setRect(SDL_Rect _rect);
			SDL_Window* getWindow() const;
		};
	}
}