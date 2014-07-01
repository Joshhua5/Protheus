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
#include "GameFileChunk.h"  
#include "..\..\util\BufferReader.h"
#include "..\..\util\BufferWriter.h"
#include "..\..\util\CBuffer.h"

namespace Pro{
	namespace IO{
		class GameFileBase
		{
		protected:
			GameFileChunk chunk;
		public:
			GameFileBase();
			~GameFileBase();
			 
			GameFileChunk getChunk();
		};
	}
}

