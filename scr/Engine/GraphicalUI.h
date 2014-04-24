#pragma once
#include <vector>
#include "GUIContext.h"
#include <SDL.h>
#include <string>

namespace Pro{
	class GraphicalUI
	{ 
		std::vector<Pro::GUI::GUIContext*> inactive_window_contexts;
		std::vector<Pro::GUI::GUIContext*> window_contexts;
	public:
		GraphicalUI();
		~GraphicalUI();

		// Passing events causes the GUI to update
		std::stack<Pro::GUI::GUIEvent> update(std::vector<SDL_Event*> events);
		Pro::GUI::GUIContext* loadGUI(const std::string& file);

		void initEditor();
		void enableEditor();
		void disableEditor();
		 
		void setActive(const std::string& name);
		void setInactive(const std::string& name);

		void addContext(Pro::GUI::GUIContext* context);
		Pro::GUI::GUIContext* getContext(ID contextID);
		void removeContext(ID);
	};

}