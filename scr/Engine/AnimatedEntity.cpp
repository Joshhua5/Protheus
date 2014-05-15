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

	void AnimatedEntity::activateAnimation(unsigned int _id){
		activeAnimation = assignedSprite.at(_id);
	}




	static int lAddAnimation(lua_State* L){
		AnimatedEntity* p = *(static_cast<AnimatedEntity**>(lua_touserdata(L, 1)));
		AnimatedSprite* i = *(static_cast<AnimatedSprite**>(lua_touserdata(L, 3)));
		p->addAnimation(static_cast<unsigned int>(lua_tonumber(L, 2)),
			i);
		return 0;
	}
	static int lActivateAnimation(lua_State* L){
		AnimatedEntity* p = *(static_cast<AnimatedEntity**>(lua_touserdata(L, 1)));
		p->activateAnimation(static_cast<unsigned int>(lua_tonumber(L, 2)));
		return 0;
	}
}