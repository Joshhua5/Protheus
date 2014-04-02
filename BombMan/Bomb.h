#include "Engine\AnimatedEntity.h"
#pragma once
class Bomb :
	public Pro::AnimatedEntity
{
public:
	bool activated;
	unsigned int timer_started;
	unsigned short countDown; 
	Bomb();
	~Bomb();
};
 