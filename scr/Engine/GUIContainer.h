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

			GUIContainer();
			~GUIContainer();
			 
			void update(SDL_Event); 
			void addComponent(GUIEntity*); 

			// LUA Functions

			static int lAddComponent(lua_State*);
		}; 
	}
}

