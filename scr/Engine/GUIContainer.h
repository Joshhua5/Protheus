#pragma once
#include "IDManager.h"
#include "GUIEntity.h"  
#include <vector>

namespace Pro{
	namespace GUI{
		class GUIContainer : 
			public GUIEntity
		{ 
			std::vector<GUIEntity*> window_entities;
			IDManager* id_manager;
		public:

			GUIContainer();
			~GUIContainer();

			void attachIDManager(IDManager*);
			void update(SDL_Event); 
			void addComponent(GUIEntity*); 
		}; 
	}
}

