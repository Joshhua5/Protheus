
#include "LuaObjectFactory.h"

using namespace Pro;
using namespace Lua;
using namespace GameObject;

int LuaObjectFactory::createScene(lua_State* L){
	auto scene = new Scene(L);
	Util::luaP_newobject<Scene>(L, scene);
	luaP_getScenes(L)->addScene(scene);
	return 1;
}
int LuaObjectFactory::createAvatar(lua_State* L){ 
	auto avatar = new Avatar(lua_tostring(L, 1));
	auto userdata = Util::luaP_newobject<Avatar>(L, avatar);
	luaP_getScenes(L)->getActiveScene()->addEntity(avatar);
	return 1;
}
int LuaObjectFactory::createCamera(lua_State* L){ 
	auto camera = new Camera(lua_tostring(L, 1));
	auto userdata = Util::luaP_newobject<Camera>(L, camera); 
	luaP_getScenes(L)->getActiveScene()->addCamera(camera);  
	return 1;
}
int LuaObjectFactory::createGUIButton(lua_State* L){
	auto button = new GUI::GUIButton(lua_tostring(L, 1));
	Util::luaP_newobject<GUI::GUIButton>(L, button); 
	return 1;
}