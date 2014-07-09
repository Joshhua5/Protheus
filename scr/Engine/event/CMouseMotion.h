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

namespace Pro{
	namespace Event{
		struct MouseMotion : IEvent{
			// Position relative to the window
			Math::Vector2 window_position;
			// Position relative to the original position
			Math::Vector2 relative_position;

			static inline string lGetMetatable(){
				return "mouse_motion_event_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
			}
		};
	}
}