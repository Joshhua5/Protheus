// Protheus File Packer.cpp : Defines the entry point for the console application.
//


#include "FileIndex.h"
#include <iostream>
using namespace std;
  
int _tmain(int argc, _TCHAR* argv[])
{  
	cout << " Protheus File Packer (c) 2014\n\n\n";
	wstring str;
	string input;
	cout << " Please enter a folder directory to package:\n--> ";
	cin >> input;
	for (char x : input)
		str += x;
	 
	FileBrowser* b = new FileBrowser();
	FileIndex* index = new FileIndex();
	b->setDir(str.substr(0, str.find_last_of(L"\\"))); 
	index->createIndex(b, str.substr(str.find_last_of(L"\\") + 1, str.length()));
	index->createFile(); 
	return 0;
} 