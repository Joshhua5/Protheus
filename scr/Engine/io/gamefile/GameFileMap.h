/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Class to process the map data from the file, provides a translation layer
between GameObject::Map and the Game File

In the Game File a map is defined as a 2D array, the first byte defines
the size of of each tile in bytes.
-------------------------------------------------------------------------
History:
- 27:05:2014: Waring J.
*************************************************************************/
#pragma once

#include "..\..\gameobject\map.h"
#include "GameFileBase.h"

namespace Pro{
	namespace IO{
		using namespace GameObject;

		class GameFileMap :
			public GameFileBase
		{
			// contains the information about the map
			Map* m_map;
		public:
			GameFileMap(Map*);
			GameFileMap(){}

			// stores a map into a gamefile chunk
			void pack(Map*);
			// loads a map from a gamefile chunk
			void unpack(GameFileChunk& chunk);

			Map* getMap();
		};
	}
}
