#include "LuaObjectFactory.h"

using namespace Pro;
using namespace Lua;


int LuaObjectFactory::createAvatar(lua_State* L){
	GameObject::Avatar** userdata = Util::luaP_newuserdata<GameObject::Avatar>(L);
	luaP_getScenes(L)->getActiveScene()->addEntity(*userdata); 
	Util::luaP_setmetatable(L, GameObject::Avatar::lGetMetatable());
	return 1;
}
int LuaObjectFactory::createCamera(lua_State* L){
	Scene::Camera** userdata = Util::luaP_newuserdata<Scene::Camera>(L);
	luaP_getScenes(L)->getActiveScene()->addCamera(*userdata); 
	Util::luaP_setmetatable(L, Scene::Camera::lGetMetatable());
	return 1;
}
int LuaObjectFactory::createGUIButton(lua_State* L){
	GUI::GUIButton** userdata = Util::luaP_newuserdata<GUI::GUIButton>(L);
	Util::luaP_setmetatable(L, GUI::GUIButton::lGetMetatable());
	return 1;
}