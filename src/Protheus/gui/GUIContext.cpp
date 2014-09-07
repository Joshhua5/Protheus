#include "GUIContext.h"

using namespace Pro;
using namespace GUI;
using namespace Math;
using namespace Util;
using namespace std;

GUIContext::GUIContext(const std::string& name) : CGUID(name){}

GUIContext::GUIContext() : CGUID(){}

void GUIContext::update(SDL_Event event){
	// Don't update if context isn't active
	if (isActive()) 
		for each(auto window in windows)
			if (event.type == SDL_EventType::SDL_MOUSEBUTTONDOWN)
				if (window.second.isClickWithin(Vector2(event.button.x, event.button.y)))
					window.second.update(event);
}

void GUIContext::attachWindow(GUIWindow& window){
	// Attach all objects with an ID
	windows.insert({ window.getGUID(), window });
}
void GUIContext::detachWindow(game_id i){
	windows.erase(i);
}

unordered_map<game_id, GUIWindow>& GUIContext::getWindows()
{
	return windows;
}

string GUIContext::getContextName(){
	return GUIDLookup::getName(guid);
}

int GUIContext::lAttachWindow(lua_State* L){
	const auto p = luaP_touserdata<GUIContext>(L, 1);
	const auto w = luaP_touserdata<GUIWindow>(L, 1);
	p->attachWindow(*w);
	return 0;
}

int GUIContext::lDetachWindow(lua_State* L){
	const auto p = luaP_touserdata<GUIContext>(L, 1);
	p->detachWindow(static_cast<game_id>(lua_tonumber(L, 2)));
	return 0;
}

int GUIContext::lGetContextName(lua_State* L){
	const auto p = luaP_touserdata<GUIContext>(L, 1);
	lua_pushstring(L, p->getContextName().data());
	return 1;
}