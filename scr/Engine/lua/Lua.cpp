
#include "Lua.h"

namespace Pro{
	namespace Lua{
		CLua::CLua(){
			lua_state = luaL_newstate();
			luaL_openlibs(lua_state);  

			luaP_setScenes(lua_state, new SceneContainer());
			luaP_setNetwork(lua_state, new Networking::Network());
			luaP_setEventHandler(lua_state, new EventHandler()); 
			luaP_setSpriteManager(lua_state, new Graphics::SpriteManager()); 
			luaP_setTimer(lua_state, new Util::Timer()); 
			luaP_setFileSystem(lua_state, new Util::FileSystem()); 

			registerGlobals(lua_state);
			LuaObjectFactory(lua_state);
		}
		CLua::~CLua() { lua_close(lua_state); }

		inline void CLua::checkError(bool error){
			if (error)
				std::cout << "Lua Error: " << lua_tostring(lua_state, -1) << std::endl;
		} 

		void CLua::loadConfig(const std::string& _path){
			// The config file must be next to the executable
			// to define the root file paths

			checkError(luaL_dofile(lua_state, &_path[0]));

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
		}
		void CLua::loadResources(){
			// grab the path to the resource.lua
			lua_getglobal(lua_state, "resource_path");
			// append the path onto the root
			std::string path =
				luaP_getFileSystem(lua_state)->getRootDir() +
				lua_tostring(lua_state, -1);
			// execute file
			checkError(luaL_dofile(lua_state, &path[0]));
		}
		void CLua::loadMain(){
			// grab the path to the main.lua
			lua_getglobal(lua_state, "main_path"); 
			// append the path onto the root
			std::string path =
				luaP_getFileSystem(lua_state)->getRootDir() +
				lua_tostring(lua_state, -1);
			// execute file
			checkError(luaL_dofile(lua_state, &path[0]));
		} 
	}
}