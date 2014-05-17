#pragma once
#include <vector>
#include <SDL.h>
#include <string>
#include "GUIContext.h"

namespace Pro{
	class GraphicalUI
	{ 
		std::vector<GUI::GUIContext*> inactive_window_contexts;
		std::vector<GUI::GUIContext*> window_contexts;
	public:
		GraphicalUI();
		~GraphicalUI();

		// Passing events causes the GUI to update
		void update(std::vector<SDL_Event*> events);
		GUI::GUIContext* loadGUI(const std::string& file);

		void initEditor();
		void enableEditor();
		void disableEditor();
		 
		void setActive(const std::string& name);
		void setInactive(const std::string& name);

		void addContext(GUI::GUIContext* context);
		GUI::GUIContext* getContext(uint32 contextID);
		void removeContext(uint32);
	};

}