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
#include "..\event\EventHandler.h"
#include "..\graphics\SpriteManager.h"
#include "..\gameobject\Scene.h"

namespace Pro{
	namespace Lua{
		class LuaGlobalFactory
		{
		public:
			LuaGlobalFactory(lua_State* L){
				const luaL_Reg globalFunctions[] = {
						{ "passBackToEngine", &EventHandler::lUpdate }
				};

				for each(auto i in globalFunctions)
					lua_register(L, i.name, i.func);

				for (int x = 0; x < 10; ++x)
					lua_pushstring(L, "");

				lua_setglobal(L, "script_engine_mode");
				lua_setglobal(L, "root_path");
				lua_setglobal(L, "resource_path");
				lua_setglobal(L, "main_path");
				lua_setglobal(L, "window_title");
				lua_setglobal(L, "screen_width");
				lua_setglobal(L, "screen_height");
				lua_setglobal(L, "screen_position_x");
				lua_setglobal(L, "screen_position_y");
				lua_setglobal(L, "fullscreen");
			}
			LuaGlobalFactory(){};
			~LuaGlobalFactory(){};
		};
	}
}
