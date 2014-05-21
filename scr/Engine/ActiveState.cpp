#include "ActiveState.h"

using namespace Pro;
using namespace Component;

ActiveState::ActiveState()
{
}


ActiveState::~ActiveState()
{
}




void ActiveState::activate(){
	active = true;
}
void ActiveState::deactivate(){
	active = false;
}
bool ActiveState::isActive(){
	return active;
}

// LUA Functions


int ActiveState::lActivate(lua_State* L){
	(*static_cast<ActiveState**>(lua_touserdata(L, 1)))->activate();
	return 0;
}
int ActiveState::lDeactivate(lua_State* L){
	(*static_cast<ActiveState**>(lua_touserdata(L, 1)))->deactivate();
	return 0;
}
int ActiveState::lIsActive(lua_State* L){
	lua_pushboolean(L, (*static_cast<ActiveState**>(lua_touserdata(L, 1)))->isActive());
	return 1;
}
