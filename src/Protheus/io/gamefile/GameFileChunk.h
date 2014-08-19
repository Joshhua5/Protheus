/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Data hiding of a game file chunk
-------------------------------------------------------------------------
History:
- 25:06:2014: Waring J.
*************************************************************************/
#pragma once

#include "..\..\util\CBuffer.h"
#include "..\..\util\BufferReader.h"
#include "..\..\util\BufferWriter.h"
#include "EGameFileChunkType.h"
#include <string>

namespace Pro{
	namespace IO{
		using namespace std;
		class GameFileChunk
		{
		public:
			string chunkName;
			EChunkType chunkType;
			CBuffer chunkData;
		};
	}
}