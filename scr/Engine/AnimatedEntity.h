#pragma once
#include "AnimatedSprite.h"
#include "Entity.h" 
#include <unordered_map>

namespace Pro{
	 
	class AnimatedEntity : 
		public Entity
	{
		std::unordered_map<unsigned int , AnimatedSprite*> assignedSprite;

	public:
		AnimatedEntity();
		~AnimatedEntity();

		AnimatedSprite* activeAnimation;
		void addAnimation(unsigned int animID, AnimatedSprite* sprite);
		void activateAnimation(unsigned int animID);

		// LUA FUNCTIONS
		
		static int lAddAnimation(lua_State*);
		static int lActivateAnimation(lua_State*);
	};

}