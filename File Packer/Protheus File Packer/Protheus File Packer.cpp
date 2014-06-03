// Protheus File Packer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileIndex.h"
using namespace std;

// pass -i "folder path" 

// returns the size of the folder



// Get's the file loaded into the fdd
void* getFile(std::wstring path, long size){
	std::fstream stream(path);
	char* buffer = new char[size];
	stream.readsome(buffer, size);
	stream.close();
	return buffer;
} 

int _tmain(int argc, _TCHAR* argv[])
{ 
	if (argc != 2)
		return 1;
	wstring str(argv[1]);
	FileBrowser* b = new FileBrowser();
	FileIndex* index = new FileIndex();
	b->setDir(str.substr(0, str.find_last_of(L"\\"))); 
	index->createIndex(b, str.substr(str.find_last_of(L"\\") + 1, str.length()));

	return 0;
} 