
#include "Element.h"
#include <Windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <fstream>
#include <string> 
#include <stack>
#include <string>



using namespace std; 

class FileBrowser{
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;  
	HANDLE hFind = INVALID_HANDLE_VALUE; 
	TCHAR currentPath[MAX_PATH];
public:
	void setDir(wstring);

	void up();
	// enteres the folder
	void enter(wstring);
	// get Directory
	wstring getDir();
	stack<WIN32_FIND_DATA> getFileList();
	stack<wstring> getFolderList();
};

