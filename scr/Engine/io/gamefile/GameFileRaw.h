/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	A raw chunk, has no understanding of the data it contains,
	A buffer will be returned of the data contained inside the chunk
-------------------------------------------------------------------------
History:
- 02:07:2014: Waring J.
*************************************************************************/
#pragma once

#include "GameFileBase.h"

namespace Pro{
	namespace IO{
		using namespace std;

		class GameFileRaw :
			public GameFileBase
		{
		protected:
			CBuffer* data;
		public:
			GameFileRaw(const string& name, CBuffer* buffer); 
			GameFileRaw(GameFileChunk& chunk);
			GameFileRaw(){}

			// Packs a CBuffer into a chunk
			void pack(const string& name, CBuffer* buffer);
			void pack(string&& name, CBuffer* buffer);

			// extracts a CBuffer from the chunk
			void unpack(GameFileChunk& chunk);

			// returns the unpacked data
			CBuffer* getData();
		}; 
	}
}
