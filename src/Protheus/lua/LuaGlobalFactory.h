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
#include "..\ScriptGame.h"

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
						{ "GameState", &GameState::lCreate },
						{ "getID", &GUIDLookup::lGetGUID },
						{ "getName", &GUIDLookup::lGetName }, 
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

			void setGlobals(ScriptGame& game, lua_State* lua_state){
				// Pass pointers do we don't copy the class, otherwise they're copied
				// and lua will contain reference to a different instance than Protheus.
				luaP_newobject<ScriptGame>(lua_state, &game); 
				lua_setglobal(lua_state, "Game");

				luaP_newobject<StateStack>(lua_state, &game.stack);
				lua_setglobal(lua_state, "Stack");

				luaP_newobject<SpriteBatcher>(lua_state, luaP_getRenderer(lua_state)->getBatcher());
				lua_setglobal(lua_state, "SpriteBatcher");

				luaP_newobject<Renderer>(lua_state, luaP_getRenderer(lua_state));
				lua_setglobal(lua_state, "Renderer");

				luaP_newobject<SpriteManager>(lua_state, luaP_getSpriteManager(lua_state));
				lua_setglobal(lua_state, "SpriteManager");

				luaP_newobject<EventHandler>(lua_state, luaP_getEventHandler(lua_state));
				lua_setglobal(lua_state, "EventHandler");

			//	luaP_newobject<Timer>(lua_state, luaP_getTimer(lua_state));
			//	lua_setglobal(lua_state, "Timer");

	/*			luaP_newobject<Network>(lua_state, luaP_getNetwork(lua_state));
				lua_setglobal(lua_state, "Network");*/

				luaP_newobject<TextRenderer>(lua_state, luaP_getTextRenderer(lua_state));
				lua_setglobal(lua_state, "Text"); 
			}
		};
	}
}
