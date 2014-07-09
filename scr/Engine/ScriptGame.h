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
#include "util\LuaUtils.h"

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
	};
}
