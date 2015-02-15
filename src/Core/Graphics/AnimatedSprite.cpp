#include "AnimatedSprite.h"

using namespace Pro;
using namespace Graphics;

AnimatedSprite::AnimatedSprite(){}
AnimatedSprite::~AnimatedSprite(){}

void AnimatedSprite::addFrame(Sprite&& sprite){
	frames_.push_back(sprite);
}

unsigned AnimatedSprite::getFrameCount() const{
	return frames_.size();
}

Sprite& AnimatedSprite::GetFrame(unsigned index){
	return frames_.at(index);
} 