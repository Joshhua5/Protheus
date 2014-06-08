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

#include "GUIDLookup.h"
#include "GUIEntity.h"

namespace Pro{
	namespace GUI{
		class GUIContainer : 
			public GUIEntity
		{ 
			std::vector<GUIEntity*> window_entities; 
		public:
			GUIContainer(const std::string& name);
			GUIContainer();
			~GUIContainer();
			 
			void update(SDL_Event); 
			void addComponent(GUIEntity*); 

			// LUA Functions

			static int lAddComponent(lua_State*);

			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "gui_container_metatable";
			}
		}; 
	}
}

