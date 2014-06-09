/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description: 

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.
*************************************************************************/


#include "EntityContainer.h"

using namespace Pro;

void EntityContainer::addEntity(GameObject::Entity* e){
	stored_entities.insert({ e->getGUID(), e });
}

GameObject::Entity* EntityContainer::getEntity(uint32  i){
	return stored_entities.at(i);
}

void EntityContainer::destoryEntity(uint32  i){
	stored_entities.erase(i);
}
