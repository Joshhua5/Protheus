/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
 
-------------------------------------------------------------------------
History:
- 27:05:2014: Waring J.
*************************************************************************/
#pragma once

#include "GameFileBase.h"
#include "..\..\util\LuaUtils.h"
#include "..\..\graphics\Renderer.h"

namespace Pro{
	namespace IO{
		using namespace Asset;
		class GameFileImage :
			public GameFileBase
		{
			uint32 sprite;
		public:
			//GameFileImage(Sprite* sprite); 
			GameFileImage(){}
			  
			// unpacks a chunk and loads the sprite into the 
			// sprite manager, the sprite's id can be 
			// returned with getSpriteID
			void unpack(lua_State* L, GameFileChunk& chunk);

			// if the ID is 0 then there was an error in 
			// loading the sprite
			uint32 getSpriteID();
		};
	}
}
