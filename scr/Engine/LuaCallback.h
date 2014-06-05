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
#include "lua\lua.hpp"
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
		};
	}
}

