#pragma once 
#include "AnimatedEntity.h"
#include "Inventory.h"
#include "SpriteEntity.h"
namespace Pro{
	namespace GameObject{ 
		class Avatar :
			public AnimatedEntity,
			public Inventory
		{
		public:
			Avatar(const std::string& name);
			Avatar();
			~Avatar();
		};
	}
}