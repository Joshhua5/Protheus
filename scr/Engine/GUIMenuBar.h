#pragma once
#include <vector>
#include "GUIDropDownMenu.h"
#include <SDL_events.h>
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
