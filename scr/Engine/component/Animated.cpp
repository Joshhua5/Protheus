#include "Animated.h"

using namespace Pro;
using namespace Component;

game_id Animated::getAnimation() const{
	return animation_guid;
}


const char* Animated::getAnimationName() const{
	return animation_name;
}

void Animated::setAnimation(const game_id animationID,const unsigned int Steps){
	animation_guid = animationID;
	max_step = Steps;
	current_step = 0;
}

unsigned int Animated::getStep() const{
	return current_step;
}

void Animated::step(){
	current_step != max_step ? current_step++ : current_step = 0;
}

int Animated::lSetAnimation(lua_State* L){
	const auto as = Util::luaP_touserdata<Animated>(L, 1);
	const auto sm = luaP_getSpriteManager(L);
	const auto anim_id = static_cast<game_id>(lua_tonumber(L, 2));
	as->setAnimation(anim_id, sm->getAnim(anim_id)->getStepCount());
	return 0;
}

int Animated::lGetAnimation(lua_State* L){
	const auto as = Util::luaP_touserdata<Animated>(L, 1);
	lua_pushnumber(L, as->getAnimation());
	return 1;
}