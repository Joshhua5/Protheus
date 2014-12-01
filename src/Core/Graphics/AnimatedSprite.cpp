#include "AnimatedSprite.h"

using namespace Pro;
using namespace Graphics;

AnimatedSprite::AnimatedSprite(){}
AnimatedSprite::~AnimatedSprite(){}

void AnimatedSprite::addFrame(Sprite&& sprite){
	frames.push_back(sprite);
}

unsigned AnimatedSprite::getFrameCount() const{
	return frames.size();
}

Sprite& AnimatedSprite::getFrame(unsigned index){
	return frames.at(index);
} 