#include "LuaObjectFactory.h"

using namespace Pro;
using namespace Lua;


void LuaObjectFactory::defineMetatable(lua_State* L, string table_name, vector<luaL_Reg> fields){
	luaL_newmetatable(L, &table_name[0]);

	for each(auto field in fields){
		lua_pushcfunction(L, field.func);
		lua_setfield(L, -2, field.name);
	}

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
}

//  Purpose of this function is to reduce the dupilicated function defitions between metatables
template<typename T> vector<luaL_Reg> createMetatable(CLASS_FLAG objectFlags){
	vector<luaL_Reg> out;
	if (objectFlags & CLASS_FLAG::ACTIVE_STATE){
		out.push_back({ "activate", &T::lActivate });
		out.push_back({ "deactivate", &T::lDeactivate });
		out.push_back({ "isActive", &T::lIsActive });
	}
	else if (objectFlags & CLASS_FLAG::AREA){ 
		out.push_back({ "getDimensions", &T::lGetDimensions });
		out.push_back({ "setDimensions", &T::lSetDimensions });
	}
	else if (objectFlags & CLASS_FLAG::GUID){
		out.push_back({ "getGUID", &T::lGetGUID });
	}
	else if (objectFlags & CLASS_FLAG::SCRIPTABLE){
		out.push_back({ "update", &T::lUpdate });
		out.push_back({ "attachFunction", &T::lAttachFunction });
	}
	else if (objectFlags & CLASS_FLAG::LUA_CALLBACK){ 
		out.push_back({ "bindCallback", &T::lBindCallback }); 
	}
	else if (objectFlags & CLASS_FLAG::NAME){
		// No Lua Functions
	}
	else if (objectFlags & CLASS_FLAG::POSITION){
		out.push_back({ "getPosition", &T::lGetPosition });
		out.push_back({ "setPosition", &T::lSetPosition });
	}
	else if (objectFlags & CLASS_FLAG::ENTITY){
		// No Lua Functions
	}
	else if (objectFlags & CLASS_FLAG::INVENTORY){
		out.push_back({ "checkForItem", &T::lCheckForItem });
		out.push_back({ "giveItem", &T::lGiveItem });
		out.push_back({ "inventorySize", &T::lInventorySize });
		out.push_back({ "takeItem", &T::lTakeItem });
	}
	else if (objectFlags & CLASS_FLAG::ITEM){
		// No Lua Functions
	}
	else if (objectFlags & CLASS_FLAG::SPRITE_ENTITY){
		out.push_back({ "setSprite", &T::lSetSprite });
		out.push_back({ "getSprite", &T::lGetSprite });
	}
	else if (objectFlags & CLASS_FLAG::ANIMATED_ENTITY){
		out.push_back({ "activateAnimation", &T::lActivateAnimation });
		out.push_back({ "addAnimation", &T::lAddAnimation });
	}
	else if (objectFlags & CLASS_FLAG::AVATAR){
		// No Lua Functions
	}
	else if (objectFlags & CLASS_FLAG::ANIMATED_SPRITE){
		// No Lua Functions
	}
	else if (objectFlags & CLASS_FLAG::SPRITE){
		// No Lua Functions
	}
	else if (objectFlags & CLASS_FLAG::CAMERA){
		out.push_back({ "move", &T::lMove });
	}
	else if (objectFlags & CLASS_FLAG::GUI_BUTTON){
		// No Lua Functions
	}
	else if (objectFlags & CLASS_FLAG::GUI_ENTITY){
		out.push_back({ "getParent", &T::lGetParent });
		out.push_back({ "setParent", &T::lSetParent });
	}
	return out;
}  

void LuaObjectFactory::defineMetatables(lua_State* L){
	const luaL_Reg globalFunctions[] = {
			{ "avatar_create", &LuaObjectFactory::createAvatar },
			{ "camera_create", &LuaObjectFactory::createCamera }
	};

	for each(auto i in globalFunctions)
		lua_register(L, i.name, i.func);


	defineMetatable(L, "avatar_metatable",
		createMetatable<GameObject::Avatar>(
		CLASS_FLAG::GUID |
		CLASS_FLAG::ACTIVE_STATE |
		CLASS_FLAG::INVENTORY |
		CLASS_FLAG::POSITION | 
		CLASS_FLAG::ANIMATED_ENTITY |
		CLASS_FLAG::ENTITY | 
		CLASS_FLAG::AVATAR
		)); 

	defineMetatable(L, "camera_metatable",
		createMetatable<Scene::Camera>(
		CLASS_FLAG::GUID | 
		CLASS_FLAG::POSITION |
		CLASS_FLAG::AREA |
		CLASS_FLAG::CAMERA
		)); 


	defineMetatable(L, "gui_button_metatable",
		createMetatable<GUI::GUIButton>(
		CLASS_FLAG::LUA_CALLBACK | 
		CLASS_FLAG::AREA | 
		CLASS_FLAG::GUID | 
		CLASS_FLAG::ACTIVE_STATE |
		CLASS_FLAG::POSITION | 
		CLASS_FLAG::GUI_BUTTON | 
		CLASS_FLAG::GUI_ENTITY
		)); 

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
	luaP_getScenes(L)->getActiveScene()->addEntity(*userdata); 
	Util::luaP_setmetatable(L, "avatar_metatable");
	return 1;
}
int LuaObjectFactory::createCamera(lua_State* L){
	Scene::Camera** userdata = Util::luaP_newuserdata<Scene::Camera>(L);
	luaP_getScenes(L)->getActiveScene()->addCamera(*userdata); 
	Util::luaP_setmetatable(L, "camera_metatable");
	return 1;
}
int LuaObjectFactory::createGUIButton(lua_State* L){
	GUI::GUIButton** userdata = Util::luaP_newuserdata<GUI::GUIButton>(L);
	Util::luaP_setmetatable(L, "gui_button_metatable");
	return 1;
}