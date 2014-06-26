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

#include "..\math\Vector2.h"
#include "IEvent.h"
#include "EKeyboardKey.h"

namespace Pro{
	namespace Event{
		struct KeyboardEvent : IEvent{
			// Key Pressed 
			EKeyboardKey key;
			// false if key is being released
			bool key_down;
			// true if the key is being released
			bool key_up;
			// true if the key hasn't been released yet
			bool repeat;

			static inline string lGetMetatable(){
				return "event_keyboard_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){ 
			}
		};
	} 
} 