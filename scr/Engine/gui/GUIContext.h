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


			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "gui_context_metatable";
			}
			template<typename T> 
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "attachWindow", (lua_CFunction)&T::lAttachWindow });
				fields.push_back({ "detachWindow", (lua_CFunction)&T::lDetachWindow });
				fields.push_back({ "getContextName", (lua_CFunction)&T::lGetContextName });
			}
		};
	}
}
