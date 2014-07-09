#include "GameFileImage.h"

using namespace Pro;
using namespace IO;

using namespace Asset;

void GameFileImage::unpack(lua_State* L, GameFileChunk& _chunk){
	const auto manager = luaP_getSpriteManager(L);
	m_sprite_id = manager->loadSprite(_chunk.chunkName, _chunk.chunkData);
}

game_id GameFileImage::getSpriteID(){
	return m_sprite_id;
}