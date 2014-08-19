#include "WindowContainer.h"

using namespace Pro;

void WindowContainer::addWindow(Window* w){
	stored_windows.insert({ w->getGUID(), w });
}
Window* WindowContainer::getWindow(game_id _guid){
	return stored_windows.at(_guid);
}
void WindowContainer::destoryWindow(game_id _guid){
	stored_windows.erase(_guid);
}

int WindowContainer::lSetActiveWindow(lua_State* L){
	Util::luaP_registerstore(L, "ACTIVE_WINDOW", Util::luaP_touserdata<Window>(L, 2));
	return 0;
}

int WindowContainer::lGetWindow(lua_State* L){
	WindowContainer* p = Util::luaP_touserdata<WindowContainer>(L, 1);
	p->getWindow(static_cast<game_id>(lua_tonumber(L, 2)));
	return 1;
}
int WindowContainer::lAddWindow(lua_State* L){
	WindowContainer* p = Util::luaP_touserdata<WindowContainer>(L, 1);
	p->addWindow(Util::luaP_touserdata<Window>(L, 2));
	return 0;
}
int WindowContainer::lDestroyWindow(lua_State* L){
	WindowContainer* p = Util::luaP_touserdata<WindowContainer>(L, 1);
	p->destoryWindow(static_cast<game_id>(lua_tonumber(L, 2)));
	return 0;
}