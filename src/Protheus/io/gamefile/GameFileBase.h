/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	A chunk's name isn't to be written into the data
	A chunk's name is to be stores inside the m_chunk.name
	The size of the 
-------------------------------------------------------------------------
History:
- 27:05:2014: Waring J.
*************************************************************************/
#pragma once

#include <vector>
#include <string>
#include "GameFileChunk.h"
#include "..\..\util\BufferReader.h"
#include "..\..\util\BufferWriter.h"
#include "..\..\util\CBuffer.h"

namespace Pro{
	namespace IO{
		class GameFileBase
		{
		protected:
			GameFileChunk m_chunk;

		public:
			GameFileBase(){};

			GameFileChunk getChunk();

			// Returns the chunk, but ruins the internal
			// chunk
			GameFileChunk extractChunk();

			const std::string getName();
		};
	}
}
