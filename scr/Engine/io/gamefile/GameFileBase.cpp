#include "GameFileBase.h"

using namespace Pro;
using namespace IO;

GameFileBase::GameFileBase()
{
}


GameFileBase::~GameFileBase()
{
}
 
GameFileChunk GameFileBase::getChunk(){
	return chunk;
}