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
				{ "getGUID", &GameObject::Avatar::lGetGUID },
				{ "getPosition", &GameObject::Avatar::lGetPosition },
				{ "setPosition", &GameObject::Avatar::lSetPosition },
				{ "activate", &GameObject::Avatar::lActivate },
				{ "deactivate", &GameObject::Avatar::lDeactivate },
				{ "isActive", &GameObject::Avatar::lIsActive },
				{ "giveItem", &GameObject::Avatar::lGiveItem },
				{ "takeItem", &GameObject::Avatar::lTakeItem },
				{ "inventorySize", &GameObject::Avatar::lInventorySize },
				{ "checkForItem", &GameObject::Avatar::lCheckForItem },
				{ "addAnimation", &GameObject::Avatar::lAddAnimation },
				{ "activateAnimation", &GameObject::Avatar::lActivateAnimation },
				{ nullptr, nullptr }
			}; defineMetatable("avatar_metatable", avatar_metatable);

			luaL_Reg camera_metatable[] = {
				{ "getPosition", &Scene::Camera::lGetPosition },
				{ "setPosition", &Scene::Camera::lSetDimensions },
				{ "getDimensions", &Scene::Camera::lGetDimensions },
				{ "setDimensions", &Scene::Camera::lSetDimensions },
				{ "move", &Scene::Camera::lMove },
				{ nullptr, nullptr }
			}; defineMetatable("camera_metatable", camera_metatable);

			luaL_Reg engine_metatable[] = {
				{ "update", &Core::lUpdate },
				{ nullptr, nullptr }
			}; defineMetatable("engine_metatable", engine_metatable);

			luaL_Reg gui_button_metatable[] = {
				{ "bindCallback", &GUI::GUIButton::lBindCallback },
				{ "getPosition", &GUI::GUIButton::lGetPosition },
				{ "setPosition", &GUI::GUIButton::lSetPosition },
				{ "getDimensions", &GUI::GUIButton::lGetDimensions },
				{ "setDimensions", &GUI::GUIButton::lSetDimensions },
				{ "getParent", &GUI::GUIButton::lGetParent },
				{ "setParent", &GUI::GUIButton::lSetParent },
				{ "getName", &GUI::GUIButton::lGetName }, 
				{ "getGUID", &GUI::GUIButton::lGetGUID },
				{ "enable", &GUI::GUIButton::lEnable },
				{ "disable", &GUI::GUIButton::lDisable },
				{ "isEnabled", &GUI::GUIButton::lIsEnabled },
				{ nullptr, nullptr }
			}; defineMetatable("gui_entity_metatable", gui_button_metatable);

			luaL_Reg gui_context_metatable[] = {
				{ "attachWindow", &GUI::GUIContext::lAttachWindow },
				{ "detachWindow", &GUI::GUIContext::lDetachWindow }, 
				{ "getContextName", &GUI::GUIContext::lGetContextName },
				{ nullptr, nullptr }
			}; defineMetatable("gui_context_metatable", gui_context_metatable);

			luaL_Reg gui_window_metatable[] = {
				{ "bindCallback", &GUI::GUIWindow::lBindCallback },
				{ "getPosition", &GUI::GUIWindow::lGetPosition },
				{ "setPosition", &GUI::GUIWindow::lSetPosition },
				{ "getDimensions", &GUI::GUIWindow::lGetDimensions },
				{ "setDimensions", &GUI::GUIWindow::lSetDimensions },
				{ "getParent", &GUI::GUIWindow::lGetParent },
				{ "setParent", &GUI::GUIWindow::lSetParent },
				{ "getName", &GUI::GUIWindow::lGetName }, 
				{ "getGUID", &GUI::GUIWindow::lGetGUID },
				{ "enable", &GUI::GUIWindow::lEnable },
				{ "disable", &GUI::GUIWindow::lDisable },
				{ "isEnabled", &GUI::GUIWindow::lIsEnabled },
				{ "addComponent", &GUI::GUIWindow::lAddComponent },
				{nullptr, nullptr}

			}; defineMetatable("gui_window_metatable", gui_window_metatable);

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
			GameObject::Avatar** userdata = static_cast<GameObject::Avatar**>(lua_newuserdata(L, sizeof(GameObject::Avatar*)));
			*userdata = new GameObject::Avatar(lua_tostring(L, 1));
			engine_core->addEntity(*userdata);
			luaL_getmetatable(L, "avatar_metatable");
			lua_setmetatable(L, -2); 
			return 1;
		}
		int Lua::createCamera(lua_State* L){
			Scene::Camera** userdata = static_cast<Scene::Camera**>(lua_newuserdata(L, sizeof(Scene::Camera*)));
			*userdata = new Scene::Camera(lua_tostring(L, 1));
			engine_core->addCamera(*userdata);
			if (engine_core->getActiveCamera() == nullptr)
				engine_core->setActiveCamera(lua_tostring(L, 1));
			luaL_getmetatable(L, "camera_metatable");
			lua_setmetatable(L, -2);

			return 1;
		}
		int Lua::createGUIButton(lua_State* L){
			GUI::GUIButton** userdata = static_cast<GUI::GUIButton**>(lua_newuserdata(L, sizeof(GUI::GUIButton*)));
			*userdata = new GUI::GUIButton(lua_tostring(L, 1));
			luaL_getmetatable(L, "gui_button_metatable");
			lua_setmetatable(L, -2); 
			return 1;
		}
		//int Lua::createGUIContext(lua_State* L){}
	}
}