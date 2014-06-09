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
		protected: 
			bool active;
		public:
			Entity(const std::string& name);
			Entity();
			~Entity();

			// returns the Metatable's name assosiated with this object
			inline static string lGetMetatable(){
				return "gameobject_entity_metatable";
			}
		};
	}
}