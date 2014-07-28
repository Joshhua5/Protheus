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

		// Passing events causes the GUI to update
		void update(std::vector<SDL_Event*> events);
		GUI::GUIContext* loadGUI(const std::string& file);

		void initEditor();
		void enableEditor();
		void disableEditor();

		void setActive(const std::string& name);
		void setInactive(const std::string& name);

		void addContext(GUI::GUIContext* context);
		GUI::GUIContext* getContext(game_id contextID);
		void removeContext(game_id);

		constexpr static const char* lGetMetatable(){
			return "gui_metatable";
		}

		template<typename T>
		static inline void lGetFunctions(std::vector<luaL_Reg>& fields){  
		}
	};
}