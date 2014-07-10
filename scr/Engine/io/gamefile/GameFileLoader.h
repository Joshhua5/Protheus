/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	A GameFile class which has the base of GameFileBase, all write to their
	own buffers, once their buffer has been populated, the GameFileLoader
	will act as an api to mask the packing and unpacking
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