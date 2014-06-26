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
#include "..\io\input.h" 
#include <SDL.h>
#include "CEvent.h"
#include "..\lua\lib\lua.hpp"
#include "..\util\LuaUtils.h"



namespace Pro{
	using namespace Event;
	class EventHandler
	{
	private:
		Input* input;
		std::vector<CEvent> keyboardEvents;
		std::vector<CEvent> mouseEvents;
		std::vector<CEvent> systemEvents;
		std::vector<CEvent> userEvents;
	public:
		EventHandler();
		~EventHandler();

		void update();
		std::vector<CEvent>* EventHandler::pollKeyEvents();
		std::vector<CEvent>* EventHandler::pollMouseEvents();
		std::vector<CEvent>* EventHandler::pollSystemEvents();
		std::vector<CEvent>* EventHandler::pollUserEvents();

		static int lUpdate(lua_State*);

		static inline string lGetMetatable(){
			return "event_handler_metatable";
		}

		template<typename T>
		static inline void lGetFunctions(std::vector<luaL_Reg>& fields){  
			fields.push_back({ "update", &T::lUpdate });
		}
	}; 
}