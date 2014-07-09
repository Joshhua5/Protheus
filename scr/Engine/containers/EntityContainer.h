/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Stores entities with reference from their GUID's
Stores the 3 types of entities in seperate containers
and one general container for unknown classes with the
Entity base

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.
*************************************************************************/

#pragma once

#include <unordered_map>
#include "..\GameObjects.h"

namespace Pro{
	using namespace std;
	using namespace GameObject;

	class EntityContainer{
	protected:
		unordered_map<game_id, StaticEntity*> m_static_entities;
		unordered_map<game_id, DynamicEntity*> m_dynamic_entities;
		unordered_map<game_id, DataEntity*> m_data_entities;

		unordered_map<game_id, Entity*> m_entities;

	public:

		template<typename T>
		T* getEntity(game_id id){
			return m_entities.at(id);
		}

		template<typename T>
		void addEntity(Entity* e){
			m_entities.insert({ e->getGUID(), e });
		}

		template<typename T>
		void eraseEntity(game_id id){
			m_entities.erase(id);
		}
	};
}