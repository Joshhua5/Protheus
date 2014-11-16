/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Handles all parts of the system, loads and executes a game file
-------------------------------------------------------------------------
History:
- 20:06:2014 Waring J.

*************************************************************************/
#pragma once
#include "IGame.h"
#include "lua\LuaUtils.h"

namespace Pro{
	class DataGame :
		public IGame
	{
	private:
		lua_State* lua_state;
	public:
		DataGame(lua_State* L);
		DataGame();
		~DataGame();

		int update();
		int render();
		int initialize();
		int cleanup();
		int gameLoop();
	};
}
