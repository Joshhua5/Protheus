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

#include "Network.h"
#include "Renderer.h"
#include "SceneContainer.h"
#include "WindowContainer.h"
#include "Timer.h"
#include "EventHandler.h"

#include "lua/lua.hpp"  
#include "LuaObjectFactory.h"
#include "LuaGlobalFactory.h" 


namespace Pro{
	namespace Lua{
		static lua_State* lua_state; 
		//static const luaL_Reg entity_func [];
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
			lua_State* getLuaState();

			static int dumpStack(lua_State*);   
			void loadConfig(const std::string& path);
			void loadResources();
			void loadMain();  
		};
	}
}
 