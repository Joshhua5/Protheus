/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Execute the update and render function defined by the user in main.lua
ScriptGame will be executed if engine is in script mode.
-------------------------------------------------------------------------
History:
- 20:06:2014 Waring J.
*************************************************************************/
#pragma once
#include "IGame.h"
#include "graphics\Renderer.h"
#include "networking\CNetwork.h"
#include "util\LuaUtils.h"
#include "util\Timer.h"
#include "event\EventHandler.h"

namespace Pro{
	class ScriptGame :
		public IGame
	{
	private:
		lua_State* lua_state; 
	public:
		ScriptGame(lua_State* L);
		ScriptGame();
		~ScriptGame();

		int update();
		int render();
		int initialize();
		int cleanup();
		int gameLoop();
		void exit();

		static int lGetSpriteManager(lua_State* L){
			Util::luaP_newobject<Graphics::SpriteManager>(L, luaP_getSpriteManager(L));
			return 1;
		}
		static int lGetRenderer(lua_State* L){
			const auto r = luaP_getRenderer(L);
			Util::luaP_newobject<Graphics::Renderer>(L, r);
			return 1;
		} 

		// returns the Metatable's name assosiated with this object
		static string lGetMetatable(){
			return "script_game_metatable";
		}

		template<typename T>
		static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
			fields.push_back({ "getSpriteManager", &T::lGetSpriteManager });
			fields.push_back({ "getRenderer", &T::lGetRenderer });
			fields.push_back({ "exit", &T::lExit });
		}
	};
}
