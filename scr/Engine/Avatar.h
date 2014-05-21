/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

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