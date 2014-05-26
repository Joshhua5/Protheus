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
#include <SDL_events.h>
#include <unordered_map>
#include <vector>

#include "GUIWindow.h" 
#include "CGUID.h"

namespace Pro{
	namespace GUI{
		class GUIContext : 
			public Component::CGUID
		{
		private:
			std::unordered_map<uint32, GUIWindow> windows; 

			GUIEntity* focusedEntity; 
		public:
			GUIContext(const std::string& name);
			GUIContext();
			~GUIContext();

			void update(SDL_Event event);

			void attachWindow(GUIWindow& window);
			void detachWindow(uint32);
			  
			std::string* getContextName(); 

			// LUA Functions

			static int lAttachWindow(lua_State*);
			static int lDetachWindow(lua_State*);
			static int lGetContextName(lua_State*); 

			//static int 
		};
	}
}
