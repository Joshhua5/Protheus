/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 10:06:2014: Waring J.
*************************************************************************/
#pragma once 

#include "EEvent.h" 
#include "IEvent.h"
#include "CMouseButton.h"
#include "CMouseMotion.h"
#include "CMouseWheel.h"
#include "CKeyboardEvent.h"

namespace Pro{
	namespace Event{
		struct CEvent{
			EEvent type;
			// use the type to cast the event and retrieve it's data
			IEvent* event; 

			static inline string lGetMetatable(){
				return "event_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){ 
			}
		};
	}
}
