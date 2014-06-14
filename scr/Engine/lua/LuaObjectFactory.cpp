
#include "LuaObjectFactory.h"

using namespace Pro;
using namespace Lua;
using namespace GameObject;

int LuaObjectFactory::createScene(lua_State* L){
	auto scene = new Scene(L);
	Util::luaP_newuserdata<Scene>(L, scene);
	luaP_getScenes(L)->addScene(scene); 
	Util::luaP_setmetatable(L, Scene::lGetMetatable());
	return 1;
}
int LuaObjectFactory::createAvatar(lua_State* L){ 
	auto avatar = new Avatar(lua_tostring(L, 1));
	auto test1 = lua_gettop(L);
	auto userdata = Util::luaP_newuserdata<Avatar>(L, avatar); 
	auto test2 = lua_gettop(L);
	luaP_getScenes(L)->getActiveScene()->addEntity(avatar);  
	Util::luaP_setmetatable(L, Avatar::lGetMetatable());
	auto test3 = lua_gettop(L);
	return 1;
}
int LuaObjectFactory::createCamera(lua_State* L){ 
	auto camera = new Camera(lua_tostring(L, 1));
	auto userdata = Util::luaP_newuserdata<Camera>(L, camera); 
	luaP_getScenes(L)->getActiveScene()->addCamera(camera); 
	Util::luaP_setmetatable(L, Camera::lGetMetatable());
	return 1;
}
int LuaObjectFactory::createGUIButton(lua_State* L){
	auto button = new GUI::GUIButton(lua_tostring(L, 1));
	Util::luaP_newuserdata<GUI::GUIButton>(L, button);
	Util::luaP_setmetatable(L, GUI::GUIButton::lGetMetatable());
	return 1;
}