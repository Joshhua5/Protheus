
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
	const auto a = Util::luaP_touserdata<ActiveState>(L, 1);
	a->activate();
	return 0;
}
int ActiveState::lDeactivate(lua_State* L){
	const auto a = Util::luaP_touserdata<ActiveState>(L, 1);
	a->deactivate();
	return 0;
}
int ActiveState::lIsActive(lua_State* L){
	const auto a = Util::luaP_touserdata<ActiveState>(L, 1);
	lua_pushboolean(L, a->isActive());
	return 1;
}
