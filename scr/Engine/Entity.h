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
#include <SDL_rect.h>
#include "lua\lua.hpp"
#include "Position.h"
#include "ActiveState.h"
#include "CGUID.h"

namespace Pro{
	namespace GameObject{ 

		class Entity : 
			public Component::Position,
			public Component::CGUID,
			public Component::ActiveState
		{
		protected: 
			bool active;
		public:
			Entity(const std::string& name);
			Entity();
			~Entity();

			// returns the Metatable's name assosiated with this object
			static inline string lGetMetatable(){
				return "gameobject_entity_metatable";
			}
		};
	}
}