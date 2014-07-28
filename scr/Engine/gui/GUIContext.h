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

#include <unordered_map>
#include <vector>
#include "GUIWindow.h"

namespace Pro{
	namespace GUI{
		class GUIContext :
			public Component::CGUID
		{
		private:
			std::unordered_map<game_id, GUIWindow> windows;

			GUIEntity* focusedEntity;
		public:
			GUIContext(const std::string& name);
			GUIContext();

			void update(SDL_Event event);

			void attachWindow(GUIWindow& window);
			void detachWindow(game_id);

			std::string getContextName();

			// LUA Functions

			static int lAttachWindow(lua_State*);
			static int lDetachWindow(lua_State*);
			static int lGetContextName(lua_State*);

			// returns the Metatable's name assosiated with this object
			constexpr static const char* lGetMetatable(){
				return "gui_context_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				CGUID::lGetFunctions<T>(fields);
				fields.push_back({ "attachWindow", (lua_CFunction)&T::lAttachWindow });
				fields.push_back({ "detachWindow", (lua_CFunction)&T::lDetachWindow });
				fields.push_back({ "getContextName", (lua_CFunction)&T::lGetContextName });
			}
		};
	}
}
