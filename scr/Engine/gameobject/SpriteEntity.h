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
#include "Entity.h"

using namespace std;

namespace Pro{
	namespace GameObject{
		class SpriteEntity :
			public Entity
		{
		protected:
			uint32 sprite_guid;
		public:
			SpriteEntity(const std::string& name);
			SpriteEntity();
			~SpriteEntity();

			uint32 getSpriteGUID();

			void setSprite(uint32 guid);

			// Lua Functions

			static int lSetSprite(lua_State*);
			static int lGetSpriteGUID(lua_State*);

			// returns the Metatable's name assosiated with this object
			static inline string lGetMetatable(){
				return "gameobject_sprite_entity_metatable";
			}

			template<typename T> static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				Entity::lGetFunctions<T>(fields);
				fields.push_back({ "setSprite", &T::lSetSprite });
				fields.push_back({ "getSprite", &T::lGetSpriteGUID });
			}
		};
	}
}