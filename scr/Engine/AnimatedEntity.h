#pragma once
#include <unordered_map>

#include "AnimatedSprite.h"
#include "Entity.h" 

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
		};
	}
}