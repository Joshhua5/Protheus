/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	Audio doesn't support packing, the unpacked data is unprocessed
	and is required to be processed by a IBitstreamDecoder
-------------------------------------------------------------------------
History:
- 27:05:2014: Waring J.
*************************************************************************/
#pragma once

#include "GameFileRaw.h"

namespace Pro{
	namespace IO{
		class GameFileAudio : 
			protected GameFileRaw
		{
		public:
			GameFileAudio(GameFileChunk&);
			GameFileAudio(){}

			void unpack(GameFileChunk& _chunk);
			
			CBuffer* getAudio();
		}; 
	}
}

