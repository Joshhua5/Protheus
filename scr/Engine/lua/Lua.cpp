#include "Lua.h"

using namespace Pro;
using namespace Lua;
using namespace Util;

CLua::CLua() : LuaObjectFactory(&lua_state), LuaGlobalFactory(lua_state), LuaMetatableFactory(lua_state){
	// lua_state is created in object factory
	// because it's constructer is the first called upon
	luaL_openlibs(lua_state);

	luaP_setScenes(lua_state, new SceneContainer());
	luaP_setNetwork(lua_state, new Networking::Network());
	luaP_setEventHandler(lua_state, new EventHandler()); 
	luaP_setTimer(lua_state, new Timer());
	luaP_setFileSystem(lua_state, new FileSystem());

	defineKeyTable(lua_state);
}
CLua::~CLua() { lua_close(lua_state); }

IGame* CLua::loadConfig(const std::string& _path){
	// The config file must be next to the executable
	// to define the root file paths

	Util::checkError(lua_state, luaL_dofile(lua_state, &_path[0]));

	// Create and load Window here
	// The window must be created first to register SDL_WINDOW
	// which is requied by the renderer

	// Grab the root path
	lua_getglobal(lua_state, "root_path");
	// Set the root path in the File System
	luaP_getFileSystem(lua_state)->setRootDir(lua_tostring(lua_state, -1));
	// Grab the window title
	lua_getglobal(lua_state, "window_title");
	// Create the window and set it in the registery
	luaP_setWindow(lua_state, new Window(lua_tostring(lua_state, -1), lua_state));

	// Create the renderer and set it in the registery 
	luaP_setRenderer(lua_state, new Graphics::Renderer(lua_state)); 

	luaP_setSpriteManager(lua_state, new Graphics::SpriteManager(lua_state));

	lua_getglobal(lua_state, "script_engine_mode");
	if (lua_toboolean(lua_state, -1))
		return new ScriptGame(lua_state);
	else
		return new DataGame(lua_state);
}

int CLua::isGameScriptMode(){
	lua_getglobal(lua_state, "script_engine_mode");
	return lua_toboolean(lua_state, -1);
}

void CLua::loadResources(){
	// grab the path to the resource.lua
	lua_getglobal(lua_state, "resource_path");
	// append the path onto the root
	std::string path =
		luaP_getFileSystem(lua_state)->getRootDir() +
		lua_tostring(lua_state, -1);
	// execute file
	checkError(lua_state, luaL_dofile(lua_state, &path[0]));
}

void CLua::loadMain(){
	// grab the path to the main.lua
	lua_getglobal(lua_state, "main_path");
	// append the path onto the root
	std::string path =
		luaP_getFileSystem(lua_state)->getRootDir() +
		lua_tostring(lua_state, -1);
	// execute file
	checkError(lua_state, luaL_dofile(lua_state, &path[0]));
}