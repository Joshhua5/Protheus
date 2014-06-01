/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once
#include <vector>
#include <SDL_events.h>
#include "GUIDropDownMenu.h"
namespace Pro{
	namespace GUI{
		class GUIMenuBar :
			public GUIEntity
		{
			std::vector<GUIDropDownMenu> elements;
		public:
			GUIMenuBar(const std::string& name);
			GUIMenuBar();
			~GUIMenuBar();

			void attachMenu(GUIDropDownMenu* component);
			void update(std::vector<SDL_Event> events);
		};
	}
}
