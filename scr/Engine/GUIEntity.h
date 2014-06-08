#pragma once
#include <SDL.h>
#include <string> 
#include "CGUID.h"
#include "Area.h"
#include "Position.h"
/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#include "LuaCallback.h"
#include "ActiveState.h"
#include "lua\lua.hpp"

namespace Pro{ 
	namespace GUI{ 
		 
		enum struct GUI_ENTITY_TYPE{
			BUTTON,
			LABEL,
			SLIDER,
			TEXT,
			WINDOW,
			COLLAPSIBLE_MENU
		};

		class GUIEntity : 
			public Component::CGUID,
			public Component::ActiveState,
			public Component::Area,
			public Component::Position,
			public Component::LuaCallback
		{ 
			// pointer to the container of the GUI
			GUIEntity* parent;  
			 
		public:
			// of type GUI_ENTITY_TYPE
			GUI_ENTITY_TYPE type;

			GUIEntity(const std::string& name);
			GUIEntity();
			~GUIEntity();

			// position includes the width and height
			SDL_Point getPosition();
			void setPosition(SDL_Point);

			SDL_Point getDimensions();
			void setDimensions(SDL_Point);

			GUIEntity* getParent();
			void setParent(GUIEntity*);
			    
			bool isClickWithin(Math::Vector2& v);
			bool isClickWithin(SDL_MouseButtonEvent&);
			 
			// Lua functions

			static int lGetParent(lua_State*);
			static int lSetParent(lua_State*);

			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "gui_entity_metatable";
			}

		};
	}
}