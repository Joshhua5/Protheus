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
#include "Lua.h"
#include <string>

using namespace std;

namespace Pro{
	namespace Component{
		template<typename T> class CScriptable{
			string luaFunction = "";
		public:
			CScriptable(){}
			~CScriptable(){}

			// Calls the assigned Lua Script
			void update(const string& metatable){
				lua_State* L = Lua::lua_state;
				lua_getglobal(L, &luaFunction[0]);
				T** argument = Util::luaP_newuserdata<T>(L);
				*argument = static_cast<T>(this);
				Util::luaP_setmetatable(L, &metatable[0], -2);

				if (lua_pcall(L, 1, 0, 0) != 0)
					Util::luaP_error(L);
			}
	
			// Assigns a function's name to the object
			void attachFunction(const string& function){ luaFunction = function; }

			static int lUpdate(lua_State*);
			static int lAttachFunction(lua_State*);

			// returns the Metatable assosiated with this object
			static inline string lGetMetatable(){
				return "componenet_scriptable_metatable";
			}
		}; 
	}
}
 