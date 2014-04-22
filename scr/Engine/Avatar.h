#pragma once 
#include "AnimatedEntity.h"
#include "Inventory.h"
#include "SpriteEntity.h"
namespace Pro{

	class Avatar :
		public AnimatedEntity,
		public Inventory
	{
	public:
		Avatar();
		~Avatar();
	};

}