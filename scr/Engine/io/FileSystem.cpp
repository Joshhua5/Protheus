
#include "FileSystem.h"

using namespace Pro;
using namespace Util;

FileSystem::FileSystem()
{
	rootDir = "";
}


FileSystem::~FileSystem()
{
}


CFile FileSystem::openFile(const std::string& file){ 
	return CFile(rootDir + file);
}
void FileSystem::setRootDir(const std::string& dir){
	rootDir = dir;
}
std::string& FileSystem::getRootDir(){
	return rootDir;
}