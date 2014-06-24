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
#include <string>
#include "Entity.h"

using namespace std;

namespace Pro{
	namespace GameObject{
		class Item :
			public Entity
		{
		public:
			Item(const std::string& name);
			Item();
			~Item();

			// returns the Metatable's name assosiated with this object
			static inline string lGetMetatable(){
				return "gameobject_item_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				Entity::lGetFunctions<T>(fields);
			}
		};
	}
}