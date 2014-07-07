

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
 
int LuaObjectFactory::createGUIButton(lua_State* L){
	auto button = new GUI::GUIButton(lua_tostring(L, 1));
	Util::luaP_newobject<GUI::GUIButton>(L, button); 
	return 1;
}