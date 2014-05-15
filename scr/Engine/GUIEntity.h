#pragma once
#include <SDL.h>
#include <string> 
#include "lua\lua.hpp"

namespace Pro{ 
	namespace GUI{ 

		typedef unsigned int ID;
		enum struct GUI_ENTITY_TYPE{
			BUTTON,
			LABEL,
			SLIDER,
			TEXT,
			WINDOW,
			COLLAPSIBLE_MENU
		};

		class GUIEntity
		{
			// relative position to the parent
			SDL_Point position;
			SDL_Point dimensions;
			// pointer to the container of the GUI
			GUIEntity* parent;
			// id of this entity
			// the context has a translation to the name 
			ID entityID;
			std::string entityName;
			// wether or not the entity is enabled and active
			bool entity_enabled;

			lua_State* lua_state;
			std::string lua_callback;
			unsigned char lua_arguments;
			unsigned char lua_return;
		public:
			// of type GUI_ENTITY_TYPE
			GUI_ENTITY_TYPE type;

			GUIEntity();
			~GUIEntity();

			// position includes the width and height
			SDL_Point getPosition();
			void setPosition(SDL_Point);

			SDL_Point getDimensions();
			void setDimensions(SDL_Point);

			GUIEntity* getParent();
			void setParent(GUIEntity*);

			ID getID();
			void setID(ID);

			std::string* getName();
			void setName(const std::string& name);

			bool enabled();
			void enabled(bool);

			bool isClickWithin(int x, int y);
			bool isClickWithin(SDL_MouseButtonEvent);

			std::string* getCallback();
			void setCallback(lua_State*, const std::string&, unsigned char args, unsigned char returns);
			void callback(); 

			static int lBindCallback(lua_State*);
			static int lGetPosition(lua_State*);
			static int lSetPosition(lua_State*);
			static int lGetDimensions(lua_State*);
			static int lSetDimensions(lua_State*);
			static int lGetParent(lua_State*);
			static int lSetParent(lua_State*);
			static int lGetID(lua_State*); 
			static int lSetName(lua_State*);
			static int lGetName(lua_State*);
			static int lEnable(lua_State*);
			static int lDisable(lua_State*);
			static int lIsEnabled(lua_State*);
		};
	}
}