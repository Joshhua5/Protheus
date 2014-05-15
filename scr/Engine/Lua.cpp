#include "Lua.h"

namespace Pro{
	namespace Lua{
		Lua::Lua()
		{
			lua_state = luaL_newstate();
			luaL_openlibs(lua_state);

			const luaL_Reg reg[] = { 
				{ "avatar_create", &Lua::createAvatar },
				{ "camera_create", &Lua::createCamera },
				{ "load_spritesheet", &Lua::loadSpriteSheet },
				{ "load_scene", &Lua::loadScene },
				{ "load_spriteanimations", &Lua::loadSpriteAnimations }
			};

			for each(auto i in reg)
				lua_register(lua_state, i.name, i.func);
			defineGlobals();
			defineMetatables();
		}
		Lua::~Lua()
		{
			lua_close(lua_state);
		}

		inline void Lua::checkError(bool error){
			if (error)
				std::cout << "Lua Error: " << lua_tostring(lua_state, -1) << std::endl;
		}
		int Lua::dumpStack(lua_State *L) {
			int i;
			int top = lua_gettop(L);
			for (i = 1; i <= top; i++) {  /* repeat for each level */
				int t = lua_type(L, i);
				switch (t) {

				case LUA_TSTRING:  /* strings */
					printf("`%s'", lua_tostring(L, i));
					break;

				case LUA_TBOOLEAN:  /* booleans */
					printf(lua_toboolean(L, i) ? "true" : "false");
					break;

				case LUA_TNUMBER:  /* numbers */
					printf("%g", lua_tonumber(L, i));
					break;

				default:  /* other values */
					printf("%s", lua_typename(L, t));
					break;

				}
				printf("  ");  /* put a separator */
			}
			printf("\n");  /* end the listing */
			return 0;
		}
		void Lua::defineGlobals(){
			for (int x = 0; x < 8; x++)
				lua_pushstring(lua_state, "");
			lua_setglobal(lua_state, "resource_path");
			lua_setglobal(lua_state, "main_path");
			lua_setglobal(lua_state, "title");
			lua_setglobal(lua_state, "screen_width");
			lua_setglobal(lua_state, "screen_height");
			lua_setglobal(lua_state, "screen_position_x");
			lua_setglobal(lua_state, "screen_position_y");
			lua_setglobal(lua_state, "fullscreen");
		}
		void Lua::defineMetatable(const char *table_name, luaL_Reg* fields){
			luaL_newmetatable(lua_state, &table_name[0]);
			for (int x = 0; fields[x].func != nullptr; x++){
				lua_pushcfunction(lua_state, fields[x].func);
				lua_setfield(lua_state, -2, fields[x].name);
			}

			lua_pushstring(lua_state, "__index");
			lua_pushvalue(lua_state, -2);
			lua_settable(lua_state, -3);
		}
		void Lua::defineMetatables(){
			luaL_Reg avatar_metatable[] = {
				{ "getID", &Avatar::lGetID },
				{ "getPosition", &Avatar::lGetPosition },
				{ "setPosition", &Avatar::lSetPosition },
				{ "activate", &Avatar::lActivate },
				{ "deactivate", &Avatar::lDeactivate },
				{ "isActive", &Avatar::lIsActive },
				{ "giveItem", &Avatar::lGiveItem },
				{ "takeItem", &Avatar::lTakeItem },
				{ "inventorySize", &Avatar::lInventorySize },
				{ "checkForItem", &Avatar::lCheckForItem },
				{ "addAnimation", &Avatar::lAddAnimation },
				{ "activateAnimation", &Avatar::lActivateAnimation },
				{ nullptr, nullptr }
			}; defineMetatable("avatar_metatable", avatar_metatable);

			luaL_Reg camera_metatable[] = {
				{ "getPosition", &Camera::lGetPosition },
				{ "setPosition", &Camera::lSetDimensions },
				{ "getDimensions", &Camera::lGetDimensions },
				{ "setDimensions", &Camera::lSetDimensions },
				{ "move", &Camera::lMove },
				{ nullptr, nullptr }
			}; defineMetatable("camera_metatable", camera_metatable);

			luaL_Reg engine_metatable[] = {
				{ "update", &Core::lUpdate },
				{ nullptr, nullptr }
			}; defineMetatable("engine_metatable", engine_metatable);
		}

		void Lua::attachCore(Core* _core){
			engine_core = _core;
			Core** p = (Core**)lua_newuserdata(lua_state, sizeof(Core*));
			*p = _core;
			luaL_getmetatable(lua_state, "engine_metatable");
			lua_setmetatable(lua_state, -2);
			lua_setglobal(lua_state, "engine");

		}
		void Lua::loadConfig(const std::string& path){
			checkError(luaL_dofile(lua_state, &path[0]));
		}
		void Lua::loadResources(){
			lua_getglobal(lua_state, "resource_path");
			checkError(luaL_dofile(lua_state, lua_tostring(lua_state, -1)));
		}
		void Lua::loadMain(){
			lua_getglobal(lua_state, "main_path"); 
			checkError(luaL_dofile(lua_state, lua_tostring(lua_state, -1)));
		}
		int Lua::loadSpriteSheet(lua_State* L){
			checkError(engine_core->loadSpriteSheet(lua_tostring(L, 1), lua_tostring(L, 2)));
			return 0;
		}
		int Lua::loadSpriteAnimations(lua_State* L){
			checkError(engine_core->loadSpriteAnimations(lua_tostring(L, 1)));
			return 0;
		}
		int Lua::loadScene(lua_State* L){
			checkError(engine_core->loadScene(lua_tostring(L, 1), lua_tostring(L, 2)));
			return 0;
		}

		int Lua::createAvatar(lua_State* L){
			Avatar** userdata = (Avatar**)lua_newuserdata(L, sizeof(Avatar*));
			*userdata = new Avatar();
			engine_core->addEntity(*userdata, lua_tostring(L, 1));
			luaL_getmetatable(L, "avatar_metatable");
			lua_setmetatable(L, -2);

			return 1;
		}
		int Lua::createCamera(lua_State* L){
			Camera** userdata = (Camera**)lua_newuserdata(L, sizeof(Camera*));
			*userdata = new Camera();
			engine_core->addCamera(*userdata, lua_tostring(L, 1));
			if (engine_core->getActiveCamera() == nullptr)
				engine_core->setActiveCamera(lua_tostring(L, 1));
			luaL_getmetatable(L, "camera_metatable");
			lua_setmetatable(L, -2);

			return 1;
		}

	}
}