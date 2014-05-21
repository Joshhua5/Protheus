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
#include "lua\lua.hpp"
#include "EventHandler.h"

namespace Pro{
	namespace Lua{
		class LuaGlobalFactory
		{
		public:
			LuaGlobalFactory(){};
			~LuaGlobalFactory(){};

			void registerGlobals(lua_State*);
		};
	}
}
