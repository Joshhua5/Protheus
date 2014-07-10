/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
The Animated componenet contains a id to the animatedSprite
and the current frame being drawn
-------------------------------------------------------------------------
History:
- 25:06:2014: Waring J.
*************************************************************************/
#pragma once

#include <unordered_map>
#include "..\graphics\AnimatedSprite.h"
#include "..\graphics\SpriteManager.h"

namespace Pro{
	namespace Component{
		using namespace std;
		using namespace Asset;

		class Animated
		{
		private:
			// the ID of the animation
			game_id animation_guid;

			char animation_name[32];
			// current step of the animation
			unsigned int current_step;
			// the count of steps in the animation
			unsigned int max_step;
		public:

			game_id getAnimation() const;
			const char* getAnimationName() const;

			void setAnimation(const game_id animationID,const unsigned int Steps);

			unsigned int getStep() const;
			void step();

			// LUA FUNCTIONS

			static int lGetAnimation(lua_State*);
			static int lSetAnimation(lua_State*);

			// returns the Metatable's name assosiated with this object
			static inline string lGetMetatable(){
				return "component_animated_metatable";
			}
			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "setAnimation", &T::lSetAnimation });
				fields.push_back({ "getAnimation", &T::lGetAnimation });
			}
		};
	}
}
