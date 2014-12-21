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

IO::CFile* FileSystem::openFile(const std::string& file){
	return new IO::CFile(rootDir + file);
}
void FileSystem::setRootDir(const std::string& dir){
	rootDir = dir;
}
std::string& FileSystem::getRootDir(){
	return rootDir;
}


Buffer FileSystem::getFile(const string& path) {
	auto file = openFile(path);
	auto buffer = file->read();
	delete file;
	return buffer;
}