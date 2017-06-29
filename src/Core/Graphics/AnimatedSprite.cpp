#include "AnimatedSprite.h"

using namespace Pro;
using namespace Graphics;
using namespace Util; 

void AnimatedSprite::addFrame(std::shared_ptr<Texture> sprite){
	frames_.push_back(sprite);
}

unsigned AnimatedSprite::getFrameCount() const{
	return static_cast<unsigned>(frames_.size());
}

std::shared_ptr<Texture> AnimatedSprite::GetFrame(unsigned index){
	return frames_.at(index);
} 