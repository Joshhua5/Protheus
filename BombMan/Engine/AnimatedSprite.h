#pragma once
#include "Sprite.h"
#include <vector>
namespace Pro{

	class AnimatedSprite
	{
	private:
		std::vector<Sprite*> frames;
		unsigned short currentFrame;
	public:
		AnimatedSprite();
		virtual ~AnimatedSprite();

		void nextFrame();
		Sprite* getFrame();
		Sprite* getFrame(int);
		void addFrame(Sprite*);
	};

}