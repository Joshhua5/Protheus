#include "GameFileBase.h"

using namespace Pro;
using namespace IO;
 
 
GameFileChunk GameFileBase::getChunk(){
	return chunk;
}

GameFileChunk GameFileBase::extractChunk(){
	return move(chunk);
}

const string GameFileBase::getName(){
	return chunk.chunkName;
}