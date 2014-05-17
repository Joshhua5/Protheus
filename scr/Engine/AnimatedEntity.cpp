#include "AnimatedEntity.h"
namespace Pro{
	namespace GameObject{

		AnimatedEntity::AnimatedEntity(const std::string& name) : 
			Entity(name)
		{
		}

		AnimatedEntity::AnimatedEntity() : 
			Entity()
		{
		}


		AnimatedEntity::~AnimatedEntity()
		{
		}

		void AnimatedEntity::addAnimation(unsigned int _id, Asset::AnimatedSprite* sprite){
			assignedSprite.insert({ _id, sprite });
		}

		void AnimatedEntity::activateAnimation(unsigned int _id){
			activeAnimation = assignedSprite.at(_id);
		}




		int AnimatedEntity::lAddAnimation(lua_State* L){
			AnimatedEntity* p = *(static_cast<AnimatedEntity**>(lua_touserdata(L, 1)));
			Asset::AnimatedSprite* i = *(static_cast<Asset::AnimatedSprite**>(lua_touserdata(L, 3)));
			p->addAnimation(static_cast<unsigned int>(lua_tonumber(L, 2)),
				i);
			return 0;
		}
		int AnimatedEntity::lActivateAnimation(lua_State* L){
			AnimatedEntity* p = *(static_cast<AnimatedEntity**>(lua_touserdata(L, 1)));
			p->activateAnimation(static_cast<unsigned int>(lua_tonumber(L, 2)));
			return 0;
		}
	}
}