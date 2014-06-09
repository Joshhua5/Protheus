/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Stores entities with reference from their GUID's

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.
*************************************************************************/

#pragma once

#include <unordered_map> 
#include <iostream>
#include <string>
#include "..\GUIDLookup.h"
#include "..\gameobject\Entity.h"


namespace Pro{   
	class EntityContainer{
	protected:
		std::unordered_map<uint32, GameObject::Entity*> stored_entities;

	public: 
		EntityContainer(){}
		~EntityContainer(){}

		void addEntity(GameObject::Entity*);

		GameObject::Entity* getEntity(uint32);

		void destoryEntity(uint32);

	};
}