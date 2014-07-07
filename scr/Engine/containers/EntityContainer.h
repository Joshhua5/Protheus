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
#include "..\GameObjects.h"

namespace Pro{   

	using namespace std;
	using namespace GameObject;

	class EntityContainer{
	protected:
		unordered_map<game_id, StaticEntity*> m_static_entities;
		unordered_map<game_id, DynamicEntity*> m_dynamic_entities;
		unordered_map<game_id, DataEntity*> m_data_entities;

	public: 
		EntityContainer(){} 

		void addEntity(GameObject::Entity*);

		GameObject::Entity* getEntity(game_id);

		void destoryEntity(game_id);

	};
}