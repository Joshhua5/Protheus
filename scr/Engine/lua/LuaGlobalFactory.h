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
#include "..\Math.h"
#include "..\graphics\SpriteManager.h"
#include "..\gameobject\Scene.h"

namespace Pro{
	namespace Lua{
		class LuaGlobalFactory
		{
			struct global{
					char* defaultValue;
					char* valueName; 
				};
		public:
			LuaGlobalFactory(lua_State* L){
				const luaL_Reg globalFunctions[] = { 
						{ "Vector2", &Vector2::lCreate},
						{ "getID", &GUIDLookup::lGetGUID },
						{ "getName", &GUIDLookup::lGetName }
				};

				for each(auto i in globalFunctions)
					lua_register(L, i.name, i.func);
				 
				const global globals [] = {
					// {Default String Value, Variable Name}
						{"", "script_engine_mode"},
						{"", "root_path"},
						//{"", "resource_file"},
						{"", "main_file"},
						{"", "window_title"},
						{"", "screen_width"},
						{"", "screen_height"},
						{"", "screen_position_x"},
						{"", "screen_position_y"},
						{"", "fullscreen"},
						{"", "isExitRequested"}
				};

				for each(const auto i in globals){
					lua_pushstring(L, i.defaultValue);
					lua_setglobal(L, i.valueName);
				}  
			}
			LuaGlobalFactory(){};
			~LuaGlobalFactory(){};
		};
	}
}
