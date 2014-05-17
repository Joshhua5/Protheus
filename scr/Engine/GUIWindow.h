#pragma once
#include <SDL_rect.h>
#include <SDL_events.h>
#include <vector> 
#include "GUIButton.h"
#include "GUISlider.h"
#include "GUIDLookup.h"
#include "GUIText.h"  
#include "GUIContainer.h"
#include "GUICollapsibleMenu.h"
#include "GUIMenuBar.h"

namespace Pro{
	namespace GUI{
		class GUIWindow : 
			public GUIContainer
		{
		private: 
			// Titles bar for drop down menus
			bool hasMenuBar;
			GUIMenuBar* menu_bar;

		public:
			GUIWindow();
			~GUIWindow();
			 
			void attachMenuBar(GUIMenuBar* component); 
		};
	}
}