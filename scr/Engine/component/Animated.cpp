#include "Animated.h"

using namespace Pro;
using namespace Component;

game_id Animated::getAnimation(){
	return activeAnimation;
}

void Animated::setAnimation(game_id animationID, unsigned int Steps){
	activeAnimation = animationID;
	maxStep = Steps;
	currentStep = 0;
}

unsigned int Animated::getStep(){
	return currentStep;
}

void Animated::step(){
	currentStep != maxStep ? currentStep++ : currentStep = 0;
}

int Animated::lSetAnimation(lua_State* L){
	const auto as = Util::luaP_touserdata<Animated>(L, 1);
	const auto sm = luaP_getSpriteManager(L);
	const auto anim_id = static_cast<game_id>(lua_tonumber(L, 2));
	as->setAnimation(anim_id, sm->getAnim(anim_id)->getStepCount());
	return 0;
}

int Animated::lGetAnimation(lua_State* L){
	auto as = Util::luaP_touserdata<Animated>(L, 1);
	lua_pushnumber(L, as->getAnimation());
	return 1;
}