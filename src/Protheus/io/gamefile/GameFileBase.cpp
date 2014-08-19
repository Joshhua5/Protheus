#include "GameFileBase.h"

using namespace Pro;
using namespace IO;

GameFileChunk GameFileBase::getChunk(){
	return m_chunk;
}

GameFileChunk GameFileBase::extractChunk(){
	return move(m_chunk);
}

const string GameFileBase::getName(){
	return m_chunk.chunkName;
}