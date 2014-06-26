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
#include "EGameFileChunkType.h"
namespace Pro{
	namespace IO{
		class GameFileChunk
		{
		public:
			int chunkID;
			EChunkType chunkType;
			CBuffer chunkData;
		};

	}
}