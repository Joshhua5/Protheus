#include "FileBrowser.h"
 
stack<wstring> FileBrowser::getFolderList(){  
	// Find the first file in the directory.
	hFind = FindFirstFile(&currentPath[0], &ffd);
	stack<wstring> out;
	while (FindNextFile(hFind, &ffd) != 0){

		// List all the files in the directory with some info about them.

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			if (ffd.cFileName[0] != L'.')
				out.push(ffd.cFileName); 
	}
	return out;
}

stack<WIN32_FIND_DATA> FileBrowser::getFileList(){
	// Find the first file in the directory.
	hFind = FindFirstFile(&currentPath[0], &ffd);
	std::stack<WIN32_FIND_DATA> out;
	while (FindNextFile(hFind, &ffd) != 0){

		// List all the files in the directory with some info about them.

		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{ 
			out.push(ffd);
		}
	}
	return out;
}
wstring FileBrowser::getDir(){
	return wstring(currentPath);
}

void FileBrowser::setDir(wstring p){
	p += L"\\*";
	memset(currentPath, 0, sizeof(wchar_t) * MAX_PATH);
	memcpy(currentPath, &p[0], sizeof(wchar_t) * p.length()); 
}

void FileBrowser::up(){
	wstring str(currentPath); 
	str = str.substr(0, str.find_last_of('\\'));
	str = str.substr(0, str.find_last_of('\\')); 
	setDir(str);
}

void FileBrowser::enter(wstring name){
	wstring str(currentPath);
	str = str.substr(0, str.find_last_of('*'));
	str += name; 
	setDir(str);
}
