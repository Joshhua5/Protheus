/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.
*************************************************************************/

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

#include <string>
#include <vector>
#include "..\Math.h"
#include "..\util\LuaUtils.h"

using namespace std;

namespace Pro{
	namespace Component{
		class Position
		{
		protected:
			Math::Vector2 position;
		public:
			Position();
			~Position();

			Math::Vector2 getPosition();
			Math::Vector2* pGetPosition();
			void setPosition(Math::Vector2&);

			// LUA Functions

			template<typename T> static int lGetPosition(lua_State* L){
				auto p = Util::luaP_touserdata<T>(L, 1);
				auto v = p->getPosition();
				lua_pushnumber(L, v.x);
				lua_pushnumber(L, v.y);
				return 0;
			}

			template<typename T> static int lSetPosition(lua_State* L){
				auto p = Util::luaP_touserdata<T>(L, 1);
				auto v = Math::Vector2(static_cast<int>(lua_tonumber(L, 2)),
					static_cast<int>(lua_tonumber(L, 3)));
				p->setPosition(v);
				return 0;
			}

			// returns the Metatable assosiated with this object
			static inline string lGetMetatable(){
				return "component_posititon_metatable";
			}

			template<typename T> static void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "getPosition", (lua_CFunction)&T::lGetPosition<T> });
				fields.push_back({ "setPosition", (lua_CFunction)&T::lSetPosition<T> });
			} 
		}; 
	}
}
