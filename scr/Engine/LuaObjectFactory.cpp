#include "LuaObjectFactory.h"

using namespace Pro;
using namespace Lua;


void LuaObjectFactory::defineMetatable(lua_State* L, const char *table_name, luaL_Reg* fields){
	luaL_newmetatable(L, &table_name[0]);
	for (int x = 0; fields[x].func != nullptr; x++){
		lua_pushcfunction(L, fields[x].func);
		lua_setfield(L, -2, fields[x].name);
	}

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
}


void LuaObjectFactory::defineMetatables(lua_State* L){
	const luaL_Reg globalFunctions[] = {
			{ "avatar_create", &LuaObjectFactory::createAvatar },
			{ "camera_create", &LuaObjectFactory::createCamera }
	};

	for each(auto i in globalFunctions)
		lua_register(L, i.name, i.func);


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
	}; defineMetatable(L, "avatar_metatable", avatar_metatable);

	luaL_Reg camera_metatable[] = {
			{ "getPosition", &Scene::Camera::lGetPosition },
			{ "setPosition", &Scene::Camera::lSetDimensions },
			{ "getDimensions", &Scene::Camera::lGetDimensions },
			{ "setDimensions", &Scene::Camera::lSetDimensions },
			{ "move", &Scene::Camera::lMove },
			{ nullptr, nullptr }
	}; defineMetatable(L, "camera_metatable", camera_metatable);

	luaL_Reg gui_button_metatable[] = {
			{ "bindCallback", &GUI::GUIButton::lBindCallback },
			{ "getPosition", &GUI::GUIButton::lGetPosition },
			{ "setPosition", &GUI::GUIButton::lSetPosition },
			{ "getDimensions", &GUI::GUIButton::lGetDimensions },
			{ "setDimensions", &GUI::GUIButton::lSetDimensions },
			{ "getParent", &GUI::GUIButton::lGetParent },
			{ "setParent", &GUI::GUIButton::lSetParent },
			{ "getGUID", &GUI::GUIButton::lGetGUID },
			{ "activate", &GUI::GUIButton::lActivate },
			{ "deactivate", &GUI::GUIButton::lDeactivate },
			{ "isActive", &GUI::GUIButton::lIsActive },
			{ nullptr, nullptr }
	}; defineMetatable(L, "gui_button_metatable", gui_button_metatable);

	luaL_Reg gui_context_metatable[] = {
			{ "attachWindow", &GUI::GUIContext::lAttachWindow },
			{ "detachWindow", &GUI::GUIContext::lDetachWindow },
			{ "getContextName", &GUI::GUIContext::lGetContextName },
			{ nullptr, nullptr }
	}; defineMetatable(L, "gui_context_metatable", gui_context_metatable);

	luaL_Reg gui_window_metatable[] = {
			{ "bindCallback", &GUI::GUIWindow::lBindCallback },
			{ "getPosition", &GUI::GUIWindow::lGetPosition },
			{ "setPosition", &GUI::GUIWindow::lSetPosition },
			{ "getDimensions", &GUI::GUIWindow::lGetDimensions },
			{ "setDimensions", &GUI::GUIWindow::lSetDimensions },
			{ "getParent", &GUI::GUIWindow::lGetParent },
			{ "setParent", &GUI::GUIWindow::lSetParent },
			{ "getGUID", &GUI::GUIWindow::lGetGUID },
			{ "activate", &GUI::GUIWindow::lActivate },
			{ "deactivate", &GUI::GUIWindow::lDeactivate },
			{ "isActive", &GUI::GUIWindow::lIsActive },
			{ "addComponent", &GUI::GUIWindow::lAddComponent },
			{ nullptr, nullptr }

	}; defineMetatable(L, "gui_window_metatable", gui_window_metatable);
}

int LuaObjectFactory::createAvatar(lua_State* L){
	GameObject::Avatar** userdata = Util::luaP_newuserdata<GameObject::Avatar>(L);
	Util::luaP_registerget<Scene::Scene>(L, "ACTIVE_SCENE")->addEntity(*userdata);
	Util::luaP_setmetatable(L, "avatar_metatable", -2);
	return 1;
}
int LuaObjectFactory::createCamera(lua_State* L){
	Scene::Camera** userdata = Util::luaP_newuserdata<Scene::Camera>(L);
	Util::luaP_registerget<Scene::Scene>(L, "ACTIVE_SCENE")->addCamera(*userdata);
	Util::luaP_setmetatable(L, "camera_metatable", -2);
	return 1;
}
int LuaObjectFactory::createGUIButton(lua_State* L){
	GUI::GUIButton** userdata = Util::luaP_newuserdata<GUI::GUIButton>(L);
	Util::luaP_setmetatable(L, "gui_button_metatable", -2);
	return 1;
}