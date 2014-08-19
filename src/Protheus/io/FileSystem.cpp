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

IO::CFile& FileSystem::openFile(const std::string& file){
	return *new IO::CFile(rootDir + file);
}
void FileSystem::setRootDir(const std::string& dir){
	rootDir = dir;
}
std::string& FileSystem::getRootDir(){
	return rootDir;
}