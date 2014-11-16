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
#include "..\lua\LuaUtils.h"

namespace Pro{
	namespace Event{
		using namespace Math;

		struct MouseMotion{
			// Position relative to the window
			Vector2 window_position;
			// Position relative to the original position
			Vector2 relative_position;

			static int lGetPosition(lua_State* L){
				const auto e = Util::luaP_touserdata<MouseMotion>(L, 1);
				//Util::luaP_newobject<Vector2>(L, e->window_position);
				return 1;
			}
			static int lGetRelativePosition(lua_State* L){
				const auto e = Util::luaP_touserdata<MouseMotion>(L, 1);
				//Util::luaP_newobject<Vector2>(L, e->relative_position);
				return 1;
			}


			/*constexpr*/ static const char* lGetMetatable(){
				return "mouse_motion_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "getPosition", &T::lGetPosition });
				fields.push_back({ "getRelPosition", &T::lGetRelativePosition });
			}
		};
	}
}