/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Provides a interface for the engine to have two classes, one for
game mode and script mode
-------------------------------------------------------------------------
History:
- 20:06:2014 Waring J.

*************************************************************************/
#pragma once
#include "IGame.h"
#include "lua\lib\lua.hpp"

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

