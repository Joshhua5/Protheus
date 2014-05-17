#include "AnimatedSprite.h"

using namespace Pro;
using namespace Asset;
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
