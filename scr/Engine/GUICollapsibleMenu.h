#pragma once
#include "GUIEntity.h" 
#include "GUIContainer.h"
#include "IDManager.h"
#include <SDL_events.h>
#include <vector>
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
