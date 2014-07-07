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
using namespace Pro::Math;

namespace Pro{
	namespace Component{
		
		class Position
		{
		protected:
			Vector2 position;
		public:
			Position();
			~Position();

			Vector2 getPosition(); 
			void setPosition(Vector2&);

			// LUA Functions

			static int lGetPosition(lua_State* L);
			static int lSetPosition(lua_State* L);

			// returns the Metatable assosiated with this object
			static inline string lGetMetatable(){
				return "component_posititon_metatable";
			}

			template<typename T> 
			static void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "getPosition", &T::lGetPosition });
				fields.push_back({ "setPosition", &T::lSetPosition });
			} 
		}; 
	}
}
