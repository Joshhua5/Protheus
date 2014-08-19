#include "CFile.h"

using namespace Pro;
using namespace IO;
using namespace std;

CFile::CFile(const std::string& filePath)
{
	m_file_path = filePath;
	file.open(filePath,
		fstream::binary |
		fstream::in |
		fstream::out);

	if (file.is_open() == false)
		error.reportError("Unable to open file: " + filePath);
}

CFile::~CFile()
{
	file.close();
}

void CFile::open(const std::string& filePath){
	m_file_path = filePath;
	file.open(filePath,
		fstream::binary |
		fstream::in |
		fstream::out);
	if (file.is_open() == false)
		error.reportError("Unable to open file: " + filePath);
}

bool CFile::isEndOfFile(){
	return file.eof();
}

void CFile::setReadPosition(unsigned int pos){
	file.seekg(pos);
}
unsigned int CFile::getReadPosition(){
	return static_cast<unsigned int>(file.tellg());
}
void CFile::setWritePosition(unsigned int pos){
	file.seekp(pos);
}
unsigned int CFile::getWritePosition(){
	return static_cast<unsigned int>(file.tellp());
}

void CFile::write(CBuffer& buf){
	file.write(buf.data<const char>(), buf.size());
} 

void CFile::write(const std::string& str){
	file.write(str.data(), str.length());
} 

CBuffer CFile::read(){
	CBuffer buf(this->getSize());
	file.read(buf.data<char>(), buf.size());
	return buf;
}

CBuffer CFile::read(unsigned int size){
	CBuffer buf(size);
	file.read(buf.data<char>(), buf.size());
	return buf;
} 

std::string CFile::readToken(const char delim){
	string s;
	getline(file, s, delim);
	return s;
}

std::string CFile::readLine(){
	string s;
	getline(file, s, '\n');
	return s;
}

std::string CFile::readString(){
	string s;
	getline(file, s, '\0');
	return s;
}

void CFile::close(){
	file.close();
}

bool CFile::isOpen(){
	return file.is_open();
}

unsigned int CFile::getSize(){
	if (!isOpen())
		return 0;
	
	const auto current_position = getReadPosition();
	file.seekg(0, std::ios::end);
	auto ending_position = getReadPosition();
	setReadPosition(current_position);
	return ending_position;
}

EFile CFile::peekError(){
	switch (file.rdstate()){
	case ifstream::goodbit:
		return flag;
		break;
	case ifstream::eofbit:
		flag = EFile::END_OF_FILE;
		break;
	case ifstream::failbit:
		flag = EFile::FILE_FAILED;
		break;
	case ifstream::badbit:
		flag = EFile::FILE_INVALID_OPERATION;
		break;
	}
	return flag;
}

EFile CFile::getError(){
	EFile f = peekError();
	flag = EFile::NO_ERROR;
	return f;
}