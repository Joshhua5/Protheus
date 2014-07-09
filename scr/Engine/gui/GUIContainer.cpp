#include "GUIContainer.h"

using namespace Pro;
using namespace GUI;
using namespace Math;

GUIContainer::GUIContainer(const std::string& name) : GUIEntity(name){}
GUIContainer::GUIContainer(){}
GUIContainer::~GUIContainer(){}

void GUIContainer::addComponent(GUIEntity* e){
	window_entities.push_back(e);
}

void GUIContainer::update(SDL_Event mouse_event){
	for each(auto entities in window_entities){
		if (entities->isActive()){
			switch (entities->type){
			case GUI_ENTITY_TYPE::COLLAPSIBLE_MENU:
				if (static_cast<GUIContainer*>(entities)->isClickWithin(Vector2(mouse_event.button.x, mouse_event.button.y)))
					static_cast<GUIContainer*>(entities)->update(mouse_event);
				break;
			default:
				if (entities->isClickWithin(Math::Vector2(mouse_event.button.x, mouse_event.button.y)))
					entities->callback();
				break;
			}
		}
	}
	// Call the containers callback
	callback();
}

int GUIContainer::lAddComponent(lua_State* L){
	GUIContainer* p = *static_cast<GUIContainer**>(lua_touserdata(L, 1));
	p->addComponent(*static_cast<GUIEntity**>(lua_touserdata(L, 2)));
	return 0;
}