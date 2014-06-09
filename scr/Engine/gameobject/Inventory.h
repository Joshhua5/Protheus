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
#include <SDL.h>
#include "Item.h"

namespace Pro{
	namespace GameObject{
		class Inventory : 
			Component::CGUID
		{
			std::vector<Item*> items;
		public:
			Inventory(const std::string& name);
			Inventory();
			~Inventory();

			bool checkForItem(Item*);
			bool giveItem(Item*);
			bool takeItem(Item*);
			size_t inventorySize();

			// LUA Functions

			static int lCheckForItem(lua_State*);
			static int lGiveItem(lua_State*);
			static int lTakeItem(lua_State*);
			static int lInventorySize(lua_State*);

			// returns the Metatable's name assosiated with this object
			static inline string lGetMetatable(){
				return "gameobject_inventory_metatable";
			}
		};
	}
}