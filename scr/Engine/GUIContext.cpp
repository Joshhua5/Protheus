#include "GUIContext.h"

using namespace Pro;
using namespace GUI;

GUIContext::GUIContext()
{
	id_manager = new IDManager();
}


GUIContext::~GUIContext()
{
	delete id_manager;
}

void GUIContext::update(SDL_Event event){
	for each(auto windows in windows)
		if (event.type == SDL_EventType::SDL_MOUSEBUTTONDOWN)
			if (windows.second.isClickWithin(event.button.x, event.button.y))
				windows.second.update(event);
}

void GUIContext::attachWindow(GUIWindow window, const std::string& windowName){
	// Attach all objects with an ID 
	windows.insert({ getID(windowName), window });
}
void GUIContext::detachWindow(const std::string& windowName){
	windows.erase(getID(windowName));
}

std::string GUIContext::getContextName(){
	return context_name;
}
void GUIContext::setContextName(const std::string& name){
	context_name = name;
}

Pro::ID GUIContext::getID(const std::string& name){
	return id_manager->getIDFromName(name);
}

void GUIContext::removeID(const std::string& name){
	id_manager->releaseID(name);
}
std::string GUIContext::getName(Pro::ID _id){
	return id_manager->getName(_id);
}

inline GUIContext* toPointer(lua_State* L){
	return *static_cast<GUIContext**>(lua_touserdata(L, 1));
}

int GUIContext::lAttachWindow(lua_State* L){
	GUIContext* p = toPointer(L);
	GUIWindow* w = *static_cast<GUIWindow**>(lua_touserdata(L, 1));
	p->attachWindow(*w, *w->getName());
	w->attachIDManager(p->id_manager);
	return 0;
}
int GUIContext::lDetachWindow(lua_State* L){
	GUIContext* p = toPointer(L);
	p->detachWindow(lua_tostring(L, 2));
	return 0;
}
int GUIContext::lGetContextName(lua_State* L){
	GUIContext* p = toPointer(L);
	lua_pushstring(L, &p->getContextName()[0]);
	return 1;
}
int GUIContext::lSetContextName(lua_State* L){
	GUIContext* p = toPointer(L);
	p->setContextName(lua_tostring(L, 2));
	return 0;
}