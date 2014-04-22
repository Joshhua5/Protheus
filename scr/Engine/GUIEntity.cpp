#include "GUIEntity.h"

using namespace Pro;

GUIEntity::GUIEntity()
{
}


GUIEntity::~GUIEntity()
{
}


SDL_Rect GUIEntity::getPosition(){
	return position;
}
void GUIEntity::setPosition(SDL_Rect _position){
	position = _position;
}

GUIEntity* GUIEntity::getParent(){
	return parent;
}
void GUIEntity::setParent(GUIEntity* _parent){
	parent = _parent;
}

ID GUIEntity::getID(){
	return entityID;
}
void GUIEntity::setID(ID _id){
	entityID = _id;
}

bool GUIEntity::enabled(){
	return entity_enabled;
}
void GUIEntity::enabled(bool _bool){
	entity_enabled = _bool;
}

bool GUIEntity::isClickWithin(int x, int y){
	if (position.x < x && position.x + position.w > x)
		if (position.y < y && position.y + position.h > y)
			return true;
	return false;
}
bool GUIEntity::isClickWithin(SDL_MouseButtonEvent _mouse){
	int x = _mouse.x;
	int y = _mouse.y;
	return isClickWithin(x, y);
}

GUIEvent* GUIEntity::generateEvent(){
	GUIEvent* newEvent = new GUIEvent();
	newEvent->entity_pointer = this;
	newEvent->entity_type = type;
	switch (type){
		case GUI_ENTITY_TYPE::BUTTON:
		case GUI_ENTITY_TYPE::LABEL:
		case GUI_ENTITY_TYPE::SLIDER:
			newEvent->event_type = GUI_EVENT::CLICK;
			break;
		case GUI_ENTITY_TYPE::TEXT:
		case GUI_ENTITY_TYPE::COLLAPSIBLE_MENU:
		case GUI_ENTITY_TYPE::WINDOW:
			newEvent->event_type = GUI_EVENT::FOCUS;
			break;
	}
	return newEvent;
}