#include "GameFileAudio.h"

using namespace Pro;
using namespace IO;

GameFileAudio::GameFileAudio(GameFileChunk& _chunk){
	unpack(_chunk);
}

void GameFileAudio::unpack(GameFileChunk& _chunk){
	data = &_chunk.chunkData;  
}

CBuffer* GameFileAudio::getAudio()
{
	return getData();
}

 


 