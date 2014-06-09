
#include "SpriteEntity.h"

using namespace Pro;
using namespace GameObject;

SpriteEntity::SpriteEntity(const std::string& name) : 
Entity(name){}

SpriteEntity::SpriteEntity() :
Entity(){}
 
SpriteEntity::~SpriteEntity() {}

uint32 SpriteEntity::getSpriteGUID(){
	return sprite_guid;
}

void SpriteEntity::setSprite(uint32 _guid){
	sprite_guid = _guid;
}

int SpriteEntity::lSetSprite(lua_State* L){
	SpriteEntity* p = *(static_cast<SpriteEntity**>(lua_touserdata(L, 1)));
	p->setSprite(static_cast<uint32>(lua_tonumber(L, 2)));
	return 0;
}

int  SpriteEntity::lGetSpriteGUID(lua_State* L){
	SpriteEntity* p = *(static_cast<SpriteEntity**>(lua_touserdata(L, 1)));
	lua_pushnumber(L, p->getSpriteGUID());
	return 1;
}