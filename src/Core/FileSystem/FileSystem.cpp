#include "FileSystem.h"

using namespace Pro;
using namespace Util;
using namespace std;

FileSystem::FileSystem(const string& root)
{
	rootDir_ = root;
}

FileSystem::~FileSystem()
{
}

IO::File* FileSystem::OpenFile(const std::string& file){
	return new IO::File(rootDir_ + file);
}
void FileSystem::rootDir(const std::string& dir){
	rootDir_ = dir;
}
std::string& FileSystem::rootDir(){
	return rootDir_;
}


Buffer FileSystem::GetFile(const string& path) {
	auto file = OpenFile(path);
	auto buffer = file->Read();
	delete file;
	return buffer;
}