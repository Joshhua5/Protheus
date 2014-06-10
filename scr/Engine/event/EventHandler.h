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
#include "Input.h" 
#include <SDL.h>
#include "..\lua\lib\lua.hpp"
#include "..\util\LuaUtils.h"

namespace Pro{

	class EventHandler
	{
	private:
		Input* input;
		std::vector<SDL_Event> keyboardEvents;
		std::vector<SDL_Event> mouseEvents;
		std::vector<SDL_Event> systemEvents;
		std::vector<SDL_Event> userEvents;
	public:
		EventHandler();
		~EventHandler();

		void update();
		std::vector<SDL_Event>* EventHandler::pollKeyEvents();
		std::vector<SDL_Event>* EventHandler::pollMouseEvents();
		std::vector<SDL_Event>* EventHandler::pollSystemEvents();
		std::vector<SDL_Event>* EventHandler::pollUserEvents();

		static int lUpdate(lua_State*);
	}; 
}