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

namespace Pro{
	namespace IO{
		class GameFileLoader
		{
		public: 
			GameFileLoader(){}

		};
	}
}
/*
Header{
	Config Script ID
	Image Index Offset : 0 if non exist
	Script Index Offset : 0 if non exist
	Audio Index Offset : 0 if non exist
	Object Index Offset : 0 if non exist
	Map Index Offset : 0 if non exist
	Raw Index Offset : 0 if non exist
	Name Index Offset : contains a translation between an ID and name in the file
}

Data{
	ChunkID
	ChunkType
	ChunkSize
} 

*/