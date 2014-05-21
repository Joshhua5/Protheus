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
#include <SDL_rect.h>
#include "Item.h"
namespace Pro{
	namespace GameObject{
		class Inventory
		{
			std::vector<Item*> items;
		public:
			Inventory();
			~Inventory();

			bool checkForItem(Item*);
			bool giveItem(Item*);
			bool takeItem(Item*);
			size_t inventorySize();

			static int lCheckForItem(lua_State*);
			static int lGiveItem(lua_State*);
			static int lTakeItem(lua_State*);
			static int lInventorySize(lua_State*);
		};
	}
}