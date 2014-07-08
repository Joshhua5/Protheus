

#include "AnimatedSprite.h"

using namespace Pro;
using namespace Asset;

AnimatedSprite::AnimatedSprite(const std::string& name) : CGUID(name){}
AnimatedSprite::AnimatedSprite() : CGUID() {}

void AnimatedSprite::addFrame(Sprite* sprite){
	frames.push_back(sprite); 
}

unsigned int AnimatedSprite::getStepCount() const{
	return frames.size();
} 

Sprite* AnimatedSprite::getFrame(game_id id) const{
	return frames.at(id);
}

int AnimatedSprite::lGetFrame(lua_State* L){
	AnimatedSprite* as = Util::luaP_touserdata<AnimatedSprite>(L, 1);
	Util::luaP_newobject<Sprite>(L, as->getFrame(
		static_cast<unsigned int>(lua_tonumber(L, 2))
		));
	return 0;
}

int AnimatedSprite::lAddFrame(lua_State* L){
	AnimatedSprite* as = Util::luaP_touserdata<AnimatedSprite>(L, 1);
	as->addFrame(Util::luaP_touserdata<Sprite>(L, 2));
	return 0;
}