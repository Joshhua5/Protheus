#pragma once
#include <SDL.h>
#include <string> 
#include "GUIEvent.h"

namespace Pro{ 
	struct GUIEvent;
	enum struct GUI_ENTITY_TYPE;

	typedef unsigned int ID;
	class GUIEntity
	{
		// relative position to the parent
		SDL_Rect position;
		// pointer to the container of the GUI
		GUIEntity* parent;
		// id of this entity
		// the context has a translation to the name 
		ID entityID; 
		// wether or not the entity is enabled and active
		bool entity_enabled; 
	public: 
		// of type GUI_ENTITY_TYPE
		GUI_ENTITY_TYPE type;

		GUIEntity();
		~GUIEntity(); 

		// position includes the width and height
		SDL_Rect getPosition();
		void setPosition(SDL_Rect);

		GUIEntity* getParent();
		void setParent(GUIEntity*);

		ID getID();
		void setID(ID);
		 
		bool enabled();
		void enabled(bool);

		bool isClickWithin(int x, int y);
		bool isClickWithin(SDL_MouseButtonEvent);

		GUIEvent* generateEvent();
	}; 
}