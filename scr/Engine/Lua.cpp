#include "Lua.h"

namespace Pro{
	namespace Lua{

		Lua::Lua()
		{
		}


		Lua::~Lua()
		{
			delete lua_state;
		}
		  
		void Lua::init(){
			lua_state = luaL_newstate();
			static const luaL_Reg lualibs[] =
			{
				{ "base", luaopen_base },
				{ NULL, NULL }
			};

			const luaL_Reg* lib = lualibs;
			for (; lib->func != NULL; lib++){
				lib->func(lua_state);
				lua_settop(lua_state, 0);
			}

			//luaL_dofile(lua_state, "Config.lua");

		}

		void Lua::cleanup(){
			lua_close(lua_state);
		}
	}
}