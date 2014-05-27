/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	Provdes an interface between the Lua API and Protheus,
	also provides the API for Protheus for scripts for Protheus

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <string>

#include "Network.h"
#include "Renderer.h"
#include "SceneContainer.h"
#include "WindowContainer.h"
#include "Timer.h"
#include "EventHandler.h"
#include "FileSystem.h"

#include "lua/lua.hpp"  
#include "LuaObjectFactory.h"
#include "LuaGlobalFactory.h" 
 
namespace Pro{
	namespace Lua{
		static lua_State* lua_state;  
		class CLua :
			public LuaObjectFactory,
			public LuaGlobalFactory
		{  
			 
			// No arguments
			static int updateEngine(lua_State*);
			static int renderEngine(lua_State*);
			static int startEngine(lua_State*);
			static int loadSpriteSheet(lua_State* L);
			static int loadSpriteAnimations(lua_State* L);
			static int loadScene(lua_State* L);
			inline static void checkError(bool error);
			 
		public:
			CLua();
			~CLua(); 
			// Returns the static lua state
			lua_State* getLuaState();
			// prints out all the contents inside of the lua stack
			static int dumpStack(lua_State*);
			// Loads the config.lua relative to the executable
			void loadConfig(const std::string& path);
			// loads the resource.lua relative to the FileSystem root
			void loadResources();
			// loads the main.lua relative to the FileSystem root
			void loadMain();  
		};
	}
}
 