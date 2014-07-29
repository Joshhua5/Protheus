/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Provides a component to attach scripts to an object
-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <string>
#include <vector>
#include "..\lua\lib\lua.hpp"
using namespace std;
namespace Pro{
	namespace Component{
		class LuaCallback
		{
			lua_State* lua_state;
			std::string lua_callback;

		public:
			LuaCallback();

			std::string* getCallback();
			void setCallback(lua_State* luaState, const std::string& functionName);
			void callback();

			// Lua functions

			static int lBindCallback(lua_State*);

			// returns the Metatable assosiated with this object
			/*constexpr*/ static const char* lGetMetatable(){
				return "component_lua_callback_metatable";
			}
			template<typename T>
			inline static void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "bindCallback", (lua_CFunction)&T::lBindCallback });
			}
		};
	}
}
