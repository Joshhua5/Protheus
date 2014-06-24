#pragma once
class GameFileLoader
{
public:
	GameFileLoader();
	~GameFileLoader();
};

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
 
Chunk types
-Image Index
-Script Index
-Audio Index
-Object Index
-Map Index
-Raw Index
-Image
-Script
-Audio
-Object
-Map
-Raw

*/