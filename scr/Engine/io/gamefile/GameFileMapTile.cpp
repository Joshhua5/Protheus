#include "GameFileMapTile.h"

using namespace Pro;
using namespace IO;
using namespace Util;

GameFileMapTile::GameFileMapTile(GameFileChunk& buffer)
{
	unpack(buffer);
}

// unpacks a chunk into tile data
void GameFileMapTile::unpack(GameFileChunk& buffer){
	unsigned int structSize = 0;
	structSize += sizeof(short);
	structSize += sizeof(bool) * 2;
	structSize += sizeof(char) * 32;

	unsigned int tileCount =
		buffer.chunkData.size() / structSize;

	BufferReader reader(&buffer.chunkData);

	for (unsigned int x = 0; x < tileCount; ++x){
		TileType tile;
		tile.tileID = reader.read<short>();
		tile.passable = reader.read<bool>();
		tile.isTextured = reader.read<bool>();
		tile.spriteName = reader.read_array<char>(32);
		m_tiles.push_back(tile);
	}
}

// packs tile data into a chunk
void GameFileMapTile::pack(vector<TileType>& _tiles){
	unsigned int structSize = 0;
	structSize += sizeof(short);
	structSize += sizeof(bool) * 2;
	structSize += sizeof(char) * 32;

	m_chunk.chunkData.init(structSize);

	BufferWriter writer(&m_chunk.chunkData);

	for each(auto tile in _tiles){
		writer.write<short>(tile.tileID);
		writer.write<bool>(tile.passable);
		writer.write<bool>(tile.isTextured);
		writer.write_array<char>(&tile.spriteName[0], 32);
	}
}

vector<TileType> GameFileMapTile::getTileData(){
	return m_tiles;
}