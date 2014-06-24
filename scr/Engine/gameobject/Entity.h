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

#include <SDL.h>
#include <string>
#include <vector>
#include "..\lua\lib\lua.hpp"
#include "..\component\Position.h"
#include "..\component\CGUID.h"
#include "..\component\ActiveState.h"

using namespace std;

namespace Pro{
	namespace GameObject{ 

		class Entity : 
			public Component::Position,
			public Component::CGUID,
			public Component::ActiveState
		{ 
		public:
			Entity(const std::string& name);
			Entity();
			~Entity();

			// returns the Metatable's name assosiated with this object
			inline static string lGetMetatable(){
				return "gameobject_entity_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				Position::lGetFunctions<T>(fields);
				CGUID::lGetFunctions<T>(fields);
				ActiveState::lGetFunctions<T>(fields);

				//fields.push_back({ "checkForItem", (lua_CFunction)&T::lCheckForItem });
			}
		};
	}
}