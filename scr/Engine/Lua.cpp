#include "Lua.h"

namespace Pro{
	namespace Lua{
		CLua::CLua(){
			lua_state = luaL_newstate();
			luaL_openlibs(lua_state); 
			   
			Util::luaP_registerstore(lua_state, "SCENES", new SceneContainer()); 
			Util::luaP_registerstore(lua_state, "NETWORK", new Networking::Network());
			Util::luaP_registerstore(lua_state, "EVENT_HANDELER", new EventHandler());
			Util::luaP_registerstore(lua_state, "SPRITE_MANAGER", new Graphics::SpriteManager());
			Util::luaP_registerstore(lua_state, "TIMER", new Util::Timer());

			registerGlobals(lua_state);
			defineMetatables(lua_state); 
		}
		CLua::~CLua() { lua_close(lua_state); }

		inline void CLua::checkError(bool error){
			if (error)
				std::cout << "Lua Error: " << lua_tostring(lua_state, -1) << std::endl;
		} 

		void CLua::loadConfig(const std::string& path){
			checkError(luaL_dofile(lua_state, &path[0]));

			// Create and load Window here
			// The window must be created first to register SDL_WINDOW
			// which is requied by the renderer
			Window* w = new Window(lua_tostring(lua_state, -1), lua_state);
			Graphics::Renderer* r = new Graphics::Renderer();
			lua_getglobal(lua_state, "window_title"); 
			Util::luaP_registerstore(lua_state, "WINDOW", w);
			Util::luaP_registerstore(lua_state, "RENDERER", r); 
		}
		void CLua::loadResources(){
			lua_getglobal(lua_state, "resource_path");
			checkError(luaL_dofile(lua_state, lua_tostring(lua_state, -1)));
		}
		void CLua::loadMain(){
			lua_getglobal(lua_state, "main_path"); 
			checkError(luaL_dofile(lua_state, lua_tostring(lua_state, -1)));
		} 
	}
}