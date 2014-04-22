#pragma once
#include "Item.h"
#include <vector>
#include <SDL_rect.h>
namespace Pro{

	class Inventory
	{
		SDL_Rect size;
		std::vector<Item*> items;
	public:
		Inventory();
		~Inventory();

		bool checkForItem(Item*);
		bool giveItem(Item*);
		bool takeItem(Item*);
	};

}