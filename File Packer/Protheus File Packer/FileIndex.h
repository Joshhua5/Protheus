#pragma once
#include <vector> 
#include <iostream>
#include "FileBrowser.h"
using namespace std;
 

class FileIndex
{
	FileBrowser* fs; 
	int processFiles();
	int processFiles(Element* parent, wstring name);
public:
	FileIndex();
	~FileIndex();

	vector<int> size;
	vector<Element*> elements;

	void createIndex(FileBrowser* _fs, wstring name);
	void createFile();
};

