#include "FileIndex.h"


FileIndex::FileIndex()
{
}


FileIndex::~FileIndex()
{
}

int FileIndex::processFiles(){ 
	stack<WIN32_FIND_DATA> files = fs->getFileList();
	int size = files.size();
	while (!files.empty()){
		WIN32_FIND_DATA d = files.top();
		// Convert file data into element
		Element* e = new Element();
		for (char x : d.cFileName)
			e->name += x;
		LARGE_INTEGER size;
		e->size = d.nFileSizeLow;
		e->type = 0;
		wstring dir = fs->getDir();
		dir = dir.substr(0, dir.find_last_of(L"*"));
		for (char x : dir)
			e->FilePath += x;
		e->FilePath += d.cFileName; 

		// remove data from stack and push to elements
		elements.push_back(e);
		files.pop();
	}
	return size;
}
 

int FileIndex::processFiles(Element* parent, wstring name){
	// Create Element
	Element* f = new Element();
	for (char x : name)
		f->name += x;
	f->size = 0;
	f->offset = 0;
	f->type = 1;
	// Add Element Here

	elements.push_back(f); 
	fs->enter(name);
	stack<wstring> folders = fs->getFolderList();
	int size = 0;
	if (folders.empty() == false){
		size += processFiles(f, folders.top());
		folders.pop();
	} 
	// if no folders or finished processing we get the file's
	f->size += processFiles();
	// update the folder size's of the parent
	if (parent != nullptr)
		parent->size += f->size + 1;
	fs->up();
	return size;
}

void FileIndex::createIndex(FileBrowser* _fs, wstring name){
	fs = _fs;
	processFiles(nullptr, name);
}