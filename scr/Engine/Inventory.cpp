#include "Inventory.h"

using namespace Pro;
using namespace GameObject;
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
size_t Inventory::inventorySize(){
	return items.size();
}

int Inventory::lCheckForItem(lua_State* L){
	Inventory* p = *(static_cast<Inventory**>(lua_touserdata(L, 1)));
	Item* i = *(static_cast<Item**>(lua_touserdata(L, 2)));
	lua_pushboolean(L, p->checkForItem(i));
	return 1;
}
int Inventory::lGiveItem(lua_State* L){
	Inventory* p = *(static_cast<Inventory**>(lua_touserdata(L, 1)));
	Item* i = *(static_cast<Item**>(lua_touserdata(L, 2)));
	lua_pushboolean(L, p->giveItem(i));
	return 1;
}
int Inventory::lInventorySize(lua_State* L){
	Inventory* p = *(static_cast<Inventory**>(lua_touserdata(L, 1)));
	lua_pushnumber(L, p->inventorySize());
	return 1;
}
int Inventory::lTakeItem(lua_State* L){
	Inventory* p = *(static_cast<Inventory**>(lua_touserdata(L, 1)));
	Item* i = *(static_cast<Item**>(lua_touserdata(L, 2)));
	lua_pushboolean(L, p->takeItem(i));
	return 1;
}
