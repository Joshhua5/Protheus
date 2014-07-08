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

#include <vector>
#include "..\graphics\Sprite.h"

namespace Pro{
	namespace Asset{
		class AnimatedSprite : 
			public Component::CGUID
		{
		private:
			std::vector<Sprite*> frames; 
		public:
			AnimatedSprite(const std::string& name);
			AnimatedSprite();
			 
			Sprite* getFrame(game_id id) const;

			void addFrame(Sprite*);

			unsigned int getStepCount() const;
			// LUA Functions 

			//static int lNextFrame(lua_State*);
			static int lGetFrame(lua_State*);
			static int lAddFrame(lua_State*);

			// returns the Metatable's name assosiated with this object
			static inline string lGetMetatable(){
				return "gameobject_animated_sprite_metatable";
			}

			template<typename T>
			static void lGetFunctions(std::vector<luaL_Reg>& fields){
				CGUID::lGetFunctions<T>(fields); 
				fields.push_back({ "getFrame", &T::lGetFrame });
				fields.push_back({ "addFrame", &T::lAddFrame });
			}
		}; 
	}
}