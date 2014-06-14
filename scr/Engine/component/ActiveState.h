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

#include <string>
#include <vector>
#include "..\lua\lib\lua.hpp"
using namespace std;

namespace Pro{
	namespace Component{ 

		class ActiveState
		{
		protected:
			bool active;
		public:
			ActiveState();
			~ActiveState();

			void activate();
			void deactivate();
			bool isActive();

			// LUA Functions

			static int lActivate(lua_State*);
			static int lDeactivate(lua_State*);
			static int lIsActive(lua_State*);

			// returns the Metatable assosiated with this object
			static inline string lGetMetatable(){
				return "component_active_state_metatable";
			}

			template<typename T> 
			static void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "activate", (lua_CFunction)&T::lActivate });
				fields.push_back({ "deactivate", (lua_CFunction)&T::lDeactivate });
				fields.push_back({ "isActive", (lua_CFunction)&T::lIsActive });
			}

		}; 
	}
}

