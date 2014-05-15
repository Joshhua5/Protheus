#include "Entity.h"
namespace Pro{


	Entity::Entity()
	{
	}


	Entity::~Entity()
	{
	}

	int* Entity::getPos(){
		int* out = new int[2]{position.x, position.y};
		return out;
	}

	int Entity::getPosX(){
		return position.x;
	}
	int Entity::getPosY(){
		return position.y;
	}

	void Entity::setPosX(int value){
		position.x = value;
	}
	void Entity::setPosY(int value){
		position.y = value;
	}

	void Entity::setPos(int X, int Y){
		position.x = X;
		position.y = Y;
	}

	unsigned int Entity::getID(){
		return ID;
	}
	void Entity::setID(unsigned int value){
		ID = value;
	}

	SDL_Rect Entity::getPositionRect(){
		return position;
	}

	SDL_Rect* Entity::getPositionRectP(){
		return &position;
	}

	void Entity::activate(){
		active = true;
	}
	void Entity::deactivate(){
		active = false;
	}
	bool Entity::isActive(){
		return active;
	}

	// LUA Functions

	int Entity::lGetPosition(lua_State* L){
		Entity* p = *((Entity**)lua_touserdata(L, 1));
		lua_pushnumber(L, p->getPosX());
		lua_pushnumber(L, p->getPosY());
		return 2;
	}
	int Entity::lSetPosition(lua_State* L){
		Entity* p = *((Entity**)lua_touserdata(L, 1));
		p->setPos(
			static_cast<int>(lua_tonumber(L, 2)),
			static_cast<int>(lua_tonumber(L, 3))
			);
		return 0;
	}
	int Entity::lGetID(lua_State* L){
		Entity* p = *static_cast<Entity**>(lua_touserdata(L, 1));
		lua_pushnumber(L, p->getID());
		return 1;
	}
	int Entity::lActivate(lua_State* L){
		(*static_cast<Entity**>(lua_touserdata(L, 1)))->activate();
		return 0;
	}
	int Entity::lDeactivate(lua_State* L){
		(*static_cast<Entity**>(lua_touserdata(L, 1)))->deactivate();
		return 0;
	}
	int Entity::lIsActive(lua_State* L){
		lua_pushboolean(L, (*static_cast<Entity**>(lua_touserdata(L, 1)))->isActive());
		return 1;
	}
}