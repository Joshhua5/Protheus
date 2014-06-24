
#include "AnimatedSprite.h"

using namespace Pro;
using namespace Asset;

AnimatedSprite::AnimatedSprite(const std::string& name) : CGUID(name){}
AnimatedSprite::AnimatedSprite()
{
}


AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::nextFrame(){
	if (currentFrame != frames.size()){
		currentFrame++;
	}
	else{
		currentFrame = 0;
	}
}

Sprite* AnimatedSprite::getFrame(){
	return frames[currentFrame];
}

Sprite* AnimatedSprite::getFrame(int value){
	return frames[value];
}

void AnimatedSprite::addFrame(Sprite* sprite){
	frames.push_back(sprite);
}

int AnimatedSprite::lNextFrame(lua_State* L){
	AnimatedSprite* as = Util::luaP_touserdata<AnimatedSprite>(L, 1);
	as->nextFrame();
	return 0;
}

int AnimatedSprite::lGetFrame(lua_State* L){
	AnimatedSprite* as = Util::luaP_touserdata<AnimatedSprite>(L, 1);
	Util::luaP_newobject<Sprite>(L, as->getFrame());
	return 0;
}

int AnimatedSprite::lAddFrame(lua_State* L){
	AnimatedSprite* as = Util::luaP_touserdata<AnimatedSprite>(L, 1);
	as->addFrame(Util::luaP_touserdata<Sprite>(L, 2));
	return 0;
}