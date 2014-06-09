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
#include <string>
#include "..\graphics\AnimatedSprite.h"
#include "Entity.h" 

using namespace std;

namespace Pro{
	namespace GameObject{
		class AnimatedEntity :
			public Entity
		{
			std::unordered_map<unsigned int, Asset::AnimatedSprite*> assignedSprite;

		public:
			AnimatedEntity(const std::string& name);
			AnimatedEntity();
			~AnimatedEntity();

			Asset::AnimatedSprite* activeAnimation;
			void addAnimation(unsigned int animID, Asset::AnimatedSprite* sprite);
			void activateAnimation(unsigned int animID);

			// LUA FUNCTIONS

			static int lAddAnimation(lua_State*);
			static int lActivateAnimation(lua_State*);

			// returns the Metatable's name assosiated with this object
			static inline string lGetMetatable(){
				return "gameobject_animated_entity_metatable";
			}
		};
	}
}