#include "SpriteEntity.h"

namespace Pro{

	SpriteEntity::SpriteEntity()
	{
	}


	SpriteEntity::~SpriteEntity()
	{
	}

	std::string SpriteEntity::getSpriteName(){
		return sprite_name;
	}

	void SpriteEntity::setSprite(const std::string& _name){
		sprite_name = _name;
	}


	int SpriteEntity::lSetSprite(lua_State* L){
		SpriteEntity* p = *(static_cast<SpriteEntity**>(lua_touserdata(L, 1)));
		p->setSprite(lua_tostring(L, 2));
		return 0;
	}
	int lGetSpriteName(lua_State* L){
		SpriteEntity* p = *(static_cast<SpriteEntity**>(lua_touserdata(L, 1)));
		lua_pushstring(L, &p->getSpriteName()[0]);
		return 1;
	}
}