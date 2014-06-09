/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:05:2014: Waring J.
*************************************************************************/

#pragma once
#include <unordered_map>
#include "..\graphics\Window.h"
#include "..\component\CGUID.h"
#include "..\util\LuaUtils.h"

namespace Pro{
	class WindowContainer
	{
	protected:
		std::unordered_map<uint32, Window*> stored_windows;

	public:
		WindowContainer(){}
		~WindowContainer(){}

		void addWindow(Window*);
		Window* getWindow(uint32);
		void destoryWindow(uint32);


		// LUA Functions


		static int lGetWindow(lua_State*);
		static int lAddWindow(lua_State*);
		static int lDestroyWindow(lua_State*);
		static int lSetActiveWindow(lua_State* L);
	};
}
