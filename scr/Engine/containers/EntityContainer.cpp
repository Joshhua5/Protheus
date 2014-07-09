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

template<>
void EntityContainer::eraseEntity<DataEntity>(game_id  i){
	m_data_entities.erase(i);
}

template<>
void EntityContainer::eraseEntity<StaticEntity>(game_id  i){
	m_static_entities.erase(i);
}

template<>
void EntityContainer::eraseEntity<DynamicEntity>(game_id  i){
	m_dynamic_entities.erase(i);
}

template<>
void EntityContainer::addEntity<DataEntity>(Entity* e){
	m_data_entities.insert({ e->getGUID(), static_cast<DataEntity*>(e) });
}

template<>
void EntityContainer::addEntity<StaticEntity>(Entity* e){
	m_static_entities.insert({ e->getGUID(), static_cast<StaticEntity*>(e) });
}

template<>
void EntityContainer::addEntity<DynamicEntity>(Entity* e){
	m_dynamic_entities.insert({ e->getGUID(), static_cast<DynamicEntity*>(e) });
}

template<>
DataEntity* EntityContainer::getEntity<DataEntity>(game_id id){
	return m_data_entities.at(id);
}

template<>
StaticEntity* EntityContainer::getEntity<StaticEntity>(game_id id){
	return m_static_entities.at(id);
}

template<>
DynamicEntity* EntityContainer::getEntity<DynamicEntity>(game_id id){
	return m_dynamic_entities.at(id);
}