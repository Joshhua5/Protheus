#include "Textured.h"

using namespace Pro;
using namespace Component;

game_id Textured::getSprite()const {
	return sprite_guid;
}


const char* Textured::getSpriteName() const{
	return sprite_name;
}

void Textured::setSprite(const game_id _guid){
	sprite_guid = _guid;
	memcpy(
		sprite_name,
		GUIDLookup::getName(_guid).data(),
		sizeof(char) * 32); 
}

int Textured::lSetSprite(lua_State* L){
	auto p = Util::luaP_touserdata<Textured>(L, 1);
	p->setSprite(static_cast<game_id>(lua_tonumber(L, 2)));
	return 0;
}

int  Textured::lGetSpriteGUID(lua_State* L){
	auto p = Util::luaP_touserdata<Textured>(L, 1);
	lua_pushnumber(L, p->getSprite());
	return 1;
}