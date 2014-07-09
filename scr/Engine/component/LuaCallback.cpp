#include "LuaCallback.h"

using namespace Pro;
using namespace Component;
LuaCallback::LuaCallback()
{
	lua_callback = "\0";
	lua_state = nullptr;
}

std::string* LuaCallback::getCallback(){
	return &lua_callback;
}

void LuaCallback::callback(){
	if (lua_state == nullptr)
		return;
	lua_getglobal(lua_state, &lua_callback[0]);
	lua_call(lua_state, 0, 0);
	return;
}

void LuaCallback::setCallback(lua_State* L, const std::string& func){
	lua_state = L;
	lua_callback = func;
}

int  LuaCallback::lBindCallback(lua_State* L){
	const auto p = *(static_cast<LuaCallback**>(lua_touserdata(L, 1)));

	p->setCallback(L, lua_tostring(L, 2));
	return 0;
}