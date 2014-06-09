#include "GUIContext.h"

using namespace Pro;
using namespace GUI;

GUIContext::GUIContext(const std::string& name) : CGUID(name)
{ 
}


GUIContext::GUIContext()
{ 
}


GUIContext::~GUIContext()
{
}

void GUIContext::update(SDL_Event event){
	for each(auto windows in windows)
		if (event.type == SDL_EventType::SDL_MOUSEBUTTONDOWN)
			if (windows.second.isClickWithin(event.button))
				windows.second.update(event);
}

void GUIContext::attachWindow(GUIWindow& window){
	// Attach all objects with an ID 
	windows.insert({ window.getGUID(), window });
}
void GUIContext::detachWindow(uint32 i){
	windows.erase(i);
}

std::string* GUIContext::getContextName(){
	return GUIDLookup::getName(guid);
}

inline GUIContext* toPointer(lua_State* L){
	return *static_cast<GUIContext**>(lua_touserdata(L, 1));
}

int GUIContext::lAttachWindow(lua_State* L){
	GUIContext* p = toPointer(L);
	GUIWindow* w = *static_cast<GUIWindow**>(lua_touserdata(L, 1));
	p->attachWindow(*w);
	return 0;
}

int GUIContext::lDetachWindow(lua_State* L){
	GUIContext* p = toPointer(L);
	p->detachWindow(static_cast<uint32>(lua_tonumber(L, 2)));
	return 0;
}

int GUIContext::lGetContextName(lua_State* L){
	GUIContext* p = toPointer(L);
	lua_pushstring(L, &(*p->getContextName())[0]);
	return 1;
}