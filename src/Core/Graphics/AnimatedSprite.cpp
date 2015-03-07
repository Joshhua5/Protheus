#include "AnimatedSprite.h"

using namespace Pro;
using namespace Graphics;
using namespace Util; 

void AnimatedSprite::addFrame(smart_ptr<Texture> sprite){
	frames_.push_back(sprite);
}

unsigned AnimatedSprite::getFrameCount() const{
	return frames_.size();
}

Util::smart_ptr<Texture> AnimatedSprite::GetFrame(unsigned index){
	return frames_.at(index);
} 