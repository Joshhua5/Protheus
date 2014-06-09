#include "LuaGlobalFactory.h"

using namespace Pro;
using namespace Lua;
 

void LuaGlobalFactory::registerGlobals(lua_State* L){
	const luaL_Reg globalFunctions[] = {
			{ "passBackToEngine", &EventHandler::lUpdate }
	};

	for each(auto i in globalFunctions)
		lua_register(L, i.name, i.func); 

	for (int x = 0; x < 10; x++)
		lua_pushstring(L, "");

	lua_setglobal(L, "script_engine_mode");
	lua_setglobal(L, "root_path");
	lua_setglobal(L, "resource_path");
	lua_setglobal(L, "main_path");
	lua_setglobal(L, "window_title");
	lua_setglobal(L, "screen_width");
	lua_setglobal(L, "screen_height");
	lua_setglobal(L, "screen_position_x");
	lua_setglobal(L, "screen_position_y");
	lua_setglobal(L, "fullscreen");
}


