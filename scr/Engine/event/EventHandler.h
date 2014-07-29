/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
TODO: have a stack for each kind of event

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <stack>
#include <queue>
#include "..\io\input.h"
#include <SDL.h>
#include "CMouseMotion.h"
#include "CKeyboardEvent.h"
#include "CMouseButton.h"
#include "CMouseWheel.h" 
#include "..\lua\lib\lua.hpp"
#include "..\util\LuaUtils.h"

namespace Pro{
	using namespace Event;
	using namespace std;

	class EventHandler
	{    
		queue<MouseButton> mouseButton;
		queue<MouseWheel> mouseWheel;
		queue<KeyboardEvent> keyEvent;
		queue<MouseMotion> mouseMotion;
	public: 

		void update();
		queue<KeyboardEvent>* pollKey();
		queue<MouseButton>* pollMouseButton();
		queue<MouseMotion>* pollMouseMotion();
		queue<MouseWheel>* pollMouseWheel();

		static int lGetKey(lua_State*);
		static int lGetKeyHasNext(lua_State*);
		static int lGetKeyCount(lua_State*);

		static int lGetMouseButton(lua_State*);
		static int lGetMouseButtonCount(lua_State*);
		static int lGetMouseButtonHasNext(lua_State*);

		static int lGetMouseMotion(lua_State*);
		static int lGetMouseMotionCount(lua_State*);
		static int lGetMouseMotionHasNext(lua_State*);

		static int lGetMouseWheel(lua_State*);
		static int lGetMouseWheelCount(lua_State*);
		static int lGetMouseWheelHasNext(lua_State*);

		static int lKey(lua_State*); 

		/*constexpr*/ static const char* lGetMetatable(){
			return "event_handler_metatable";
		}

		template<typename T>
		static inline void lGetFunctions(std::vector<luaL_Reg>& fields){  

			fields.push_back({ "keyNext", &T::lGetKey });
			fields.push_back({ "keyHasNext", &T::lGetKeyHasNext });
			fields.push_back({ "keyCount", &T::lGetKeyCount });

			fields.push_back({ "mouseButtonNext", &T::lGetMouseButton });
			fields.push_back({ "mouseButtonHasNext", &T::lGetMouseButtonHasNext });
			fields.push_back({ "mouseButtonCount", &T::lGetMouseButtonCount });

			fields.push_back({ "mouseMotionNext", &T::lGetMouseMotion });
			fields.push_back({ "mouseMotionHasNext", &T::lGetMouseMotionHasNext });
			fields.push_back({ "mouseMotionCount", &T::lGetMouseMotionCount });

			fields.push_back({ "mouseWheelNext", &T::lGetMouseWheel });
			fields.push_back({ "mouseWheelHasNext", &T::lGetMouseWheelHasNext });
			fields.push_back({ "mouseWheelCount", &T::lGetMouseWheelCount });
		}
	};
}