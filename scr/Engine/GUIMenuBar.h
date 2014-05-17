#pragma once
#include <vector>
#include <SDL_events.h>
#include "GUIDropDownMenu.h"
namespace Pro{
	namespace GUI{
		class GUIMenuBar
		{
			std::vector<GUIDropDownMenu> elements;
		public:
			GUIMenuBar();
			~GUIMenuBar();

			void attachMenu(GUIDropDownMenu* component);
			void update(std::vector<SDL_Event> events);
		};
	}
}
