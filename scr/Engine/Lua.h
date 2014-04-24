#pragma once

extern "C"{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

namespace Pro{
	namespace Lua{
		class Lua
		{
			lua_State* lua_state;


		public:
			Lua();
			~Lua();

			void init();
			void cleanup();
		};
	}
}