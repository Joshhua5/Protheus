/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 05:06:2014 Waring J.
*************************************************************************/
#pragma once

#include "..\lua\lib\Lua.hpp"
#include "..\util\LuaUtils.h"
#include <string>
#include <vector>

using namespace std;

namespace Pro{
	namespace Component{
		class CScriptable{
			string luaFunction = "";
		public: 

			// Calls the assigned Lua Script
			void update(lua_State* L);
	
			// Assigns a function's name to the object
			void attachFunction(const string& function);

			static int lUpdate(lua_State*);
			static int lAttachFunction(lua_State*);

			// returns the Metatable assosiated with this object
			static inline string lGetMetatable(){
				return "component_scriptable_metatable";
			}
			template<typename T> 
			static void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "update", (lua_CFunction)&T::lUpdate });
				fields.push_back({ "attachFunction", (lua_CFunction)&T::lAttachFunction });
			}
		}; 
	}
}
 