#include "GameFileImage.h"

using namespace Pro;
using namespace IO;

using namespace Asset;
 
void GameFileImage::unpack(lua_State* L, GameFileChunk& _chunk){
	const auto manager = luaP_getSpriteManager(L);
	const auto renderer = luaP_getRenderer(L)->getRenderer();
	sprite = manager->loadSprite(_chunk.chunkName, _chunk.chunkData); 
}

uint32 GameFileImage::getSpriteID(){
	return sprite;
}