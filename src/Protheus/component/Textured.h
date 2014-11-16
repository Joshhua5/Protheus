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
#include "..\lua\LuaUtils.h"

namespace Pro{
	namespace Component{

		using namespace std;

		class Textured
		{
		protected:
			game_id sprite_guid;
			 
			char sprite_name[32];
		public:

			game_id getSprite() const;
			const char* getSpriteName() const;

			void setSprite(const game_id guid);

			// LUA Functions

			static int lSetSprite(lua_State*);
			static int lGetSpriteGUID(lua_State*);

			// returns the Metatable's name assosiated with this object
			/*constexpr*/ static const char* lGetMetatable(){
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
