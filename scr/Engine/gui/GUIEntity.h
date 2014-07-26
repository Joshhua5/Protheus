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
		using namespace Math;
		using namespace Component;

		enum struct GUI_ENTITY_TYPE{
			BUTTON,
			LABEL,
			SLIDER,
			TEXT,
			WINDOW,
			COLLAPSIBLE_MENU
		};

		class GUIEntity :
			public CGUID,
			public ActiveState,
			public Area,
			public Position,
			public LuaCallback
		{
			// pointer to the container of the GUI
			GUIEntity* parent;

		public:
			// of type GUI_ENTITY_TYPE
			GUI_ENTITY_TYPE type;

			GUIEntity(const std::string& name);
			GUIEntity(); 

			GUIEntity* getParent() const;
			void setParent(GUIEntity*);

			bool isClickWithin(const Vector2& v) const;

			// Lua functions

			static int lGetParent(lua_State*);
			static int lSetParent(lua_State*);
			static int lIsClickWithin(lua_State*);

			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "gui_entity_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				CGUID::lGetFunctions<T>(fields);
				Area::lGetFunctions<T>(fields);
				Position::lGetFunctions<T>(fields);
				LuaCallback::lGetFunctions<T>(fields);
				ActiveState::lGetFunctions<T>(fields);
				fields.push_back({ "isClickWithin", &T::lIsClickWithin });
				fields.push_back({ "getParent", &T::lGetParent });
				fields.push_back({ "setParent", &T::lSetParent });
			}
		};
	}
}