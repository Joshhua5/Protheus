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
#include "..\component\CGUID.h"

namespace Pro{

	class Window :  
		public Component::CGUID
	{
		lua_State* lua_state;
		SDL_Window* window;
		std::string windowTitle; 
		SDL_Rect dim;

	public:
		Window(const std::string& name, lua_State* lua_state);
		Window();
		~Window();

		void setTitle(const std::string& title);
		void setWidth(unsigned int);
		void setHeight(unsigned int);
		void setRect(SDL_Rect _rect);
		bool createWindow();
		SDL_Window* getWindow();
	};

}