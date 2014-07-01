/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 25:06:2014: Waring J.
*************************************************************************/

#pragma once

#include "GameFileChunk.h"
#include "GameFileBase.h"
#include <unordered_map>
#include <string>

namespace Pro{
	namespace IO{
		using namespace std;
		class GameFileIndex : 
			public GameFileBase
		{ 
			unordered_map<string, unsigned int> indexes; 
		public:
			GameFileIndex(GameFileChunk& chunk);
			GameFileIndex();
			~GameFileIndex();

			// packs a vector of names and offsets
			// into a game file chunk
			void pack(vector<pair<string, unsigned int>> indexes, EChunkType type);

			// reads a chunk and extracts
			// the index
			void unpack(GameFileChunk& chunk);

			// returns the offset of a chunk
			// which has the name
			unsigned int getOffset(const string& name); 
		};
	}
}