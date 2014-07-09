#include "GameFileObject.h"

using namespace Pro;
using namespace IO;
using namespace GameObject;
using namespace std;

GameFileObject::GameFileObject()
{
}

//template<>
//void GameFileObject::pack<Avatar>(const string& name, Avatar* obj){
//	// data to be stored, spriteName
//	chunk.chunkType = EChunkType::OBJECT;
//	chunk.chunkName = name;
//}
//
//template<>
//void GameFileObject::pack<SpriteEntity>(const string& name, SpriteEntity* obj){
//	// data to be stored, spriteName
//	m_chunk.chunkType = EChunkType::OBJECT;
//	m_chunk.chunkName = name;
//	m_chunk.chunkData.init(
//		&GUIDLookup::getName(obj->getSpriteGUID())[0]
//		, sizeof(char) * 32);
//}

void GameFileObject::unpack(GameFileChunk& chunk){
}

Entity* GameFileObject::getObject(){
	return object;
}