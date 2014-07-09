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

#include <vector>
#include "GameFileBase.h"
#include "..\..\gameobject\Tile.h"

namespace Pro{
	namespace IO{
		using namespace std;
		using namespace GameObject;
		class GameFileMapTile :
			public GameFileBase
		{
			vector<TileType> m_tiles;
		public:
			GameFileMapTile(GameFileChunk& buffer);
			GameFileMapTile(){}

			// unpacks a chunk into tile data
			void unpack(GameFileChunk& buffer);

			// packs tile data into a chunk
			void pack(vector<TileType>& tiles);

			vector<TileType> getTileData();
		};
	}
}
