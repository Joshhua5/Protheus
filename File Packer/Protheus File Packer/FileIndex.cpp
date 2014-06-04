#include "FileIndex.h"


FileIndex::FileIndex()
{
}


FileIndex::~FileIndex()
{
}

void FileIndex::createFile(){
	// Get the processed size of the index (elements.count * 49)

	fstream file(fs->getDir() + L"output.pro", ios::binary | ios::out);
	if (file.bad())
		return;
	char* indexBuffer = new char[elements.size() * 49];
	file.write(indexBuffer, elements.size() * 49);

	// Iterate through the index, saving files in the file and update the offset
	cout << "\n Packing Files";
	for each(Element* e in elements){
		if (e->type == 0){
			// This is a File
			fstream otherFile(e->FilePath, ios::binary);
			char* fileBuffer = new char[e->size];
			otherFile.readsome(fileBuffer, e->size);
			// Update file offset
			e->offset = file.tellp();
			file.write(fileBuffer, e->size);
			delete[] fileBuffer;
			otherFile.close();
		}
	}

	// We left a gap at the front of the file to populate with the index
	cout << "\n Writing Index";
	file.seekp(file.beg);
	for each(Element* e in elements){
		char* element = new char[49];
		memcpy(element, &e->name[0], 32);
		memcpy(element + 31, &e->size, sizeof(long));
		memcpy(element + 39, &e->offset, sizeof(long));
		element[48] = e->type; 
		file.write(element, 49);
		delete[] element;
	}

	file.close(); 
	cout << "\ Packing Complete";
	string directory;
	for (wchar_t x : fs->getDir())
		directory += x; 

	cout << "\n output: \n-->" << directory << "output.pro";
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
	while (folders.empty() == false){
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
	cout << "\n Indexing files";
	processFiles(nullptr, name);
	cout << "\n Indexing complete: " << elements.size() << " Files/Folders found";
}