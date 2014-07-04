#include "Animated.h"

using namespace Pro;
using namespace Component;


uint32 Animated::getAnimation(){
	return activeAnimation;
}

void Animated::setAnimation(uint32 animationID, unsigned int Steps){
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
	auto as = Util::luaP_touserdata<Animated>(L, 1);
	as->setAnimation(lua_tonumber(L, 2)); 
	return 0;
}

int Animated::lGetAnimation(lua_State* L){
	auto as = Util::luaP_touserdata<Animated>(L, 1); 
	lua_pushnumber(L, as->getAnimation()); 
	return 1;
}