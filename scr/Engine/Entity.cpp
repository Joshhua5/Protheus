#include "Entity.h"
using namespace Pro;
using namespace GameObject;

Entity::Entity(const std::string& name) : CGUID(name){}

Entity::Entity() : CGUID()
{
}


Entity::~Entity()
{
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
