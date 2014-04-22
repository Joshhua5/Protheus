#pragma once
#include "GUIEntity.h"
#include <SDL_events.h>
#include <vector>
namespace Pro{
	class GUICollapsibleMenu :
		public GUIEntity
	{
		std::vector<GUIEntity*> window_entities;

	public:
		GUICollapsibleMenu();
		~GUICollapsibleMenu();

		void addComponent(GUIEntity* component);
		GUIEvent* update(SDL_Event mouse_event);
	};
}
