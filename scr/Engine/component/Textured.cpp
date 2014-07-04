#include "Textured.h"

using namespace Pro;
using namespace Component;

uint32 Textured::getSprite(){
	return sprite_guid;
}

void Textured::setSprite(uint32 _guid){
	sprite_guid = _guid;
}

int Textured::lSetSprite(lua_State* L){ 
	auto p = Util::luaP_touserdata<Textured>(L, 1);
	p->setSprite(static_cast<uint32>(lua_tonumber(L, 2)));
	return 0;
}

int  Textured::lGetSpriteGUID(lua_State* L){
	auto p = Util::luaP_touserdata<Textured>(L, 1);
	lua_pushnumber(L, p->getSprite());
	return 1;
}