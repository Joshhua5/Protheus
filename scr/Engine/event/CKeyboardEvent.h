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
#include "EKeyboardKey.h"

namespace Pro{
	namespace Event{
		struct KeyboardEvent{
			// Key Pressed
			EKeyboardKey key;
			// false if key is being released
			bool key_down;
			// true if the key is being released
			bool key_up;
			// true if the key hasn't been released yet
			bool repeat;

			static int lIsDown(lua_State* L){
				const auto e = Util::luaP_touserdata<KeyboardEvent>(L, 1);
				lua_pushboolean(L, e->key_down);
				return 1;
			}
			static int lIsUp(lua_State* L){
				const auto e = Util::luaP_touserdata<KeyboardEvent>(L, 1);
				lua_pushboolean(L, e->key_up);
				return 1;
			}
			static int lGetKey(lua_State* L){
				const auto e = Util::luaP_touserdata<KeyboardEvent>(L, 1);
				luaP_pushnumber(L, e->key);
				return 1;
			}

			static int lRepeated(lua_State* L){
				const auto e = Util::luaP_touserdata<KeyboardEvent>(L, 1);
				lua_pushboolean(L, e->repeat);
				return 1;
			}

			static inline string lGetMetatable(){
				return "event_key_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "isDown", &T::lIsDown });
				fields.push_back({ "isUp", &T::lIsUp });
				fields.push_back({ "getKey", &T::lGetKey });
				fields.push_back({ "repeated", &T::lRepeated });
			}
		};
	}
}