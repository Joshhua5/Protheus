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
#include "..\..\util\BufferWriter.h"
#include "..\..\util\BufferReader.h"
#include "GameFileChunk.h"

namespace Pro{
	namespace IO{
		class GameFileMap
		{
			// contains the information about the map
			GameFileChunk dataChunk; 
		public: 
			GameFileMap(GameObject::Map*);
			GameFileMap();
			~GameFileMap();

			GameFileChunk* getChunk(); 

			// stores a map into a gamefile chunk
			void store(GameObject::Map*);
			// loads a map from a gamefile chunk
			GameObject::Map* load();
		};
	}
}


