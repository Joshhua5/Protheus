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
#include <string>
#include "Entity.h"
namespace Pro{
	namespace GameObject{
		class SpriteEntity :
			public Entity
		{
		protected:
			uint32 sprite_guid;
		public:
			SpriteEntity(const std::string& name);
			SpriteEntity();
			~SpriteEntity();

			uint32 getSpriteGUID();

			void setSprite(uint32 guid);

			static int lSetSprite(lua_State*);
			static int lGetSpriteGUID(lua_State*);
		};
	}
}