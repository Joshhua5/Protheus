#include "AnimatedEntity.h"
namespace Pro{


	AnimatedEntity::AnimatedEntity()
	{
	}


	AnimatedEntity::~AnimatedEntity()
	{
	}

	void AnimatedEntity::addAnimation(unsigned int _id, AnimatedSprite* sprite){
		assignedSprite.insert({ _id, sprite });
	}

	void AnimatedEntity::activateAnim(unsigned int _id){
		activeAnimation = assignedSprite.at(_id);
	}
}