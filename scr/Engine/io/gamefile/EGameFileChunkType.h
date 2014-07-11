/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Enumeration of chunk types
-------------------------------------------------------------------------
History:
- 25:06:2014: Waring J.
*************************************************************************/
#pragma once

namespace Pro{
	namespace IO{
		enum struct EChunkType
		{
			IMAGE_INDEX, 
			SCRIPT_INDEX, 
			AUDIO_INDEX, 
			MAP_TILE_INDEX, 
			OBJECT_INDEX, 
			MAP_INDEX, 
			RAW_INDEX, 
			IMAGE, 
			SCRIPT, 
			TILE, 
			AUDIO, 
			OBJECT, 
			MAP_DATA, 
			RAW 
		};
	}
}