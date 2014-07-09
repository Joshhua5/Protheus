/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 25:06:2014: Waring J.
*************************************************************************/
#pragma once

#include "CGUID.h"
#include "..\util\LuaUtils.h"

namespace Pro{
	namespace Component{
		class Textured
		{
		protected:
			game_id sprite_guid;
		public:

			game_id getSprite();

			void setSprite(game_id guid);

			// LUA Functions

			static int lSetSprite(lua_State*);
			static int lGetSpriteGUID(lua_State*);

			// returns the Metatable's name assosiated with this object
			static inline string lGetMetatable(){
				return "component_textured_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "setSprite", &T::lSetSprite });
				fields.push_back({ "getSprite", &T::lGetSpriteGUID });
			}
		};
	}
}
