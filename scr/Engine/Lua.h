#pragma once

 
#include "lua/lua.hpp" 
#include "Core.h"
#include "GraphicalUI.h"
#include <string>

namespace Pro{
	namespace Lua{
		static lua_State* lua_state; 
		static Core* engine_core;
		//static const luaL_Reg entity_func [];
		class Lua
		{ 
			// Object Creation
			static int createAvatar(lua_State*);
			static int createCamera(lua_State*);
			static int createGUIButton(lua_State*);
			static int createGUIContext(lua_State*);
			// No arguments
			static int updateEngine(lua_State*);
			static int loadSpriteSheet(lua_State* L);
			static int loadSpriteAnimations(lua_State* L);
			static int loadScene(lua_State* L);
			inline static void checkError(bool error);

		public:
			Lua();
			~Lua();

			static int dumpStack(lua_State*);
			void attachCore(Core* _core); 
			void defineGlobals();
			void defineMetatables();
			void defineMetatable(const char* table_name, luaL_Reg* fields);
			void loadConfig(const std::string& path);
			void loadResources();
			void loadMain(); 

		};
	}
}
 