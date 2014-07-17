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
#include "EMouseButton.h"

namespace Pro{
	namespace Event{
		struct MouseButton{
			// which button is being pressed
			EMouseButton type;
			// if the button is being pressed this is true
			bool button_down;
			// if the button is being released this is true
			bool button_up;
			// Position of the mouse relative to the window
			Math::Vector2 window_position;

			static inline string lGetMetatable(){
				return "mouse_button_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
			}
		};
	}
}