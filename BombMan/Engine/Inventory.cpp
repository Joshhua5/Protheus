#include "Inventory.h"

namespace Pro{

	Inventory::Inventory()
	{
	}


	Inventory::~Inventory()
	{
	}

	bool Inventory::checkForItem(Item* item){
		for each(Item* obj in items){
			if (obj == item)
				return true;
		}
		return false;
	}
	bool Inventory::giveItem(Item* item){
		items.push_back(item);
		return true;
	}
	bool Inventory::takeItem(Item* item){
		for (unsigned int x = 0; x < items.size(); x++){
			if (items[x] == item){
				items.erase(items.begin() + x);
				return true;
			}
		}
		return false;
	}
}