#pragma once
#include <SDL_rect.h>
#include <SDL_events.h>
#include "GUIEntity.h"
#include "GUIEvent.h"
#include "GUIButton.h"
#include "GUISlider.h"
#include "GUIText.h"
#include "GUIEvent.h"
#include "GUICollapsibleMenu.h"
#include "GUIMenuBar.h"
#include <vector>
#include <stack>
namespace Pro{
	class GUIWindow : 
		public GUIEntity
	{
	private:
		// Packed with position and size  
		std::vector<GUIEntity*> window_entities;

		// Titles bar for drop down menus
		bool hasMenuBar;
		GUIMenuBar* menu_bar;

	public:
		GUIWindow();
		~GUIWindow();
		
		void attachMenuBar(GUIMenuBar* component);
		void addComponent(GUIEntity* component);
		GUIEvent* update(SDL_Event mouse_event);
	};
}