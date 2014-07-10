#include "GameFileObject.h"

using namespace Pro;
using namespace IO;
using namespace GameObject;
using namespace std;

GameFileObject::GameFileObject()
{
} 

void GameFileObject::unpack(GameFileChunk& chunk){
}

Entity* GameFileObject::getObject(){
	return object;
}