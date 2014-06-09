/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	Creates all the global's which are accessible through Lua.
-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once
#include "lib\lua.hpp"
#include "..\io\EventHandler.h"

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
