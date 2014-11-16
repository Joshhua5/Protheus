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

#include <Vector2.h>
#include "EMouseButton.h"

namespace Pro{
	namespace Event{
		struct MouseWheel{
			// which button is being pressed
			EMouseButton type;
			// if the button is being pressed this is true
			bool scroll_down;
			// if the button is being released this is true
			bool scroll_up;

			/*constexpr*/ static const char* lGetMetatable(){
				return "mouse_wheel_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
			}
		};
	}
}