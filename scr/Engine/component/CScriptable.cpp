#include "CScriptable.h"


using namespace std;

using namespace Pro;
using namespace Component;

void CScriptable::update(lua_State* L){
	lua_getglobal(L, &luaFunction[0]);
	lua_pcall(L, 1, 0, 0);
}

void CScriptable::attachFunction(const string& function){
	luaFunction = function; 
}

int CScriptable::lUpdate(lua_State* L){
	auto scriptable = Util::luaP_touserdata<CScriptable>(L, 1);
	scriptable->update(L);
	return 0;
}

int CScriptable::lAttachFunction(lua_State* L){
	auto scriptable = Util::luaP_touserdata<CScriptable>(L, 1);
	scriptable->attachFunction(lua_tostring(L, 2));
	return 0; 
}
