#include "GUIContainer.h"

using namespace Pro;
using namespace GUI;
using namespace Math;
using namespace Util;

GUIContainer::GUIContainer(const std::string& name) : GUIEntity(name){}
GUIContainer::GUIContainer() : GUIEntity(){} 

void GUIContainer::addComponent(GUIEntity* e){
	window_entities.push_back(e);
}

void GUIContainer::update(SDL_Event mouse_event){
	for each(const auto entities in window_entities){
		if (entities->isActive()){
			switch (entities->type){
			case GUI_ENTITY_TYPE::COLLAPSIBLE_MENU:
				if (static_cast<GUIContainer*>(entities)->isClickWithin(Vector2(mouse_event.button.x, mouse_event.button.y)))
					static_cast<GUIContainer*>(entities)->update(mouse_event);
				break;
			default:
				if (entities->isClickWithin(Vector2(mouse_event.button.x, mouse_event.button.y)))
					entities->callback();
				break;
			}
		}
	}
	// Call the containers callback
	callback();
}

int GUIContainer::lAddComponent(lua_State* L){
	const auto p = luaP_touserdata<GUIContainer>(L, 1);
	p->addComponent(luaP_touserdata<GUIEntity>(L, 2));
	return 0;
}