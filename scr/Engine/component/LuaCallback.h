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
			~LuaCallback();
			 
			std::string* getCallback();
			void setCallback(lua_State*, const std::string&, unsigned char args, unsigned char returns);
			void callback();

			// Lua functions

			static int lBindCallback(lua_State*);

			// returns the Metatable assosiated with this object
			static inline string lGetMetatable(){
				return "component_lua_callback_metatable";
			}
		};
	}
}

