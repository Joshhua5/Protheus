#pragma once
#include <SDL.h>
#include <string>  
#include "..\Components.h"
/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#include "..\GUIDLookup.h"
#include "..\Components.h"
#include "..\lua\lib\lua.hpp"

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
			Math::Vector2 getPosition();
			void setPosition(Math::Vector2);

			Math::Vector2 getDimensions();
			void setDimensions(Math::Vector2);

			GUIEntity* getParent();
			void setParent(GUIEntity*);
			    
			bool isClickWithin(Math::Vector2& v);
			 
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