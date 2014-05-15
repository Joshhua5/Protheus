#include "GUICollapsibleMenu.h"
using namespace Pro;
using namespace GUI;

GUICollapsibleMenu::GUICollapsibleMenu()
{
}


GUICollapsibleMenu::~GUICollapsibleMenu()
{
}


void GUICollapsibleMenu::addComponent(GUIEntity* component){

}

void GUICollapsibleMenu::update(SDL_Event mouse_event){
	for each(auto window_ents in window_entities)
		if (window_ents->isClickWithin(mouse_event.button))
			window_ents->callback(); 
}