#include "GUIContainer.h"
namespace Pro{
	namespace GUI{ 

		GUIContainer::GUIContainer(){}
		GUIContainer::~GUIContainer(){}
		 

		void GUIContainer::attachIDManager(IDManager* _id_manager){
			id_manager = _id_manager;
		}

		void GUIContainer::addComponent(GUIEntity* e){
			e->setID(id_manager->getID(*e->getName()));
			window_entities.push_back(e);
		}

		void GUIContainer::update(SDL_Event mouse_event){
			for each(auto entities in window_entities){
				if (entities->enabled()){
					switch (entities->type){
					case GUI_ENTITY_TYPE::COLLAPSIBLE_MENU:
						if (static_cast<GUIContainer*>(entities)->isClickWithin(mouse_event.button))
							static_cast<GUIContainer*>(entities)->update(mouse_event);
						break;
					default:
						if (entities->isClickWithin(mouse_event.button))
							entities->callback();
						break;
					}
				}
			}
		}
	}
}