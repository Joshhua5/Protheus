#pragma once
#include <SDL_events.h>
#include <vector>
#include "GUIEntity.h" 
#include "GUIContainer.h"
#include "GUIDLookup.h"

namespace Pro{
	namespace GUI{
		class GUICollapsibleMenu : 
			public GUIContainer
		{
		public:
			GUICollapsibleMenu();
			~GUICollapsibleMenu(); 
		};
	}
}
