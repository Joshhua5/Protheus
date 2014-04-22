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
}