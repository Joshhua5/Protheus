#pragma once
#include <vector>
#include "GUIContext.h"
#include <SDL.h>
#include <string>

namespace Pro{
	class GUI
	{
		std::vector<GUIContext*> inactive_window_contexts;
		std::vector<GUIContext*> window_contexts; 
	public:
		GUI();
		~GUI();

		// Passing events causes the GUI to update
		std::stack<GUIEvent> update(std::vector<SDL_Event*> events);
		GUIContext* loadGUI(const std::string& file);

		void initEditor();
		void enableEditor();
		void disableEditor();
		 
		void setActive(const std::string& name);
		void setInactive(const std::string& name);

		void addContext(GUIContext* context);
		GUIContext* getContext(ID contextID);
		void removeContext(ID);
	};

}