
#include "CFile.h"

using namespace Pro;
using namespace Util;

CFile::CFile(const std::string& filePath)
{
	file.open(filePath,
		std::fstream::binary |
		std::fstream::in |
		std::fstream::out);
}

CFile::CFile()
{
}


CFile::~CFile()
{
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
	file.write(static_cast<const char*>(buf.data), buf.size);
}

void CFile::write(float f){
	file.write((char*)&f, sizeof(float));
}
void CFile::write(const std::string& str){
	file.write(&str[0], str.length());
}
void CFile::write(int f){
	file.write((char*)&f, sizeof(int));
}

CBuffer CFile::read(unsigned int size){
	CBuffer buf(size);
	file.read((char*)buf.data, buf.size);
	return buf;
}

char CFile::readByte(){
	char c;
	file.read(&c, 1);
	return c;
}

float CFile::readFloat(){
	float f;
	file.read((char*)&f, sizeof(float));
	return f;
}

int CFile::readInt(){
	int f;
	file.read((char*)&f, sizeof(float));
	return f;
}

std::string CFile::readToken(char delim){
	std::string s;
	std::getline(file, s, delim);
	return s;
}

std::string CFile::readLine(){
	std::string s;
	std::getline(file, s, '\n');
	return s;
}

std::string CFile::readString(){
	std::string s;
	std::getline(file, s, '\0');
	return s;
}

void CFile::close(){
	file.close();
}

bool CFile::isOpen(){
	return file.is_open();
}

unsigned int CFile::getSize(){
	auto current_position = getReadPosition();
	file.seekg(0, std::ios::end);
	auto ending_position = getReadPosition();
	setReadPosition(current_position);
	return ending_position;
}

EFile CFile::peekError(){
	switch (file.rdstate()){
	case std::ifstream::goodbit:
		return flag;
		break;
	case std::ifstream::eofbit:
		flag = EFile::END_OF_FILE;
		break;
	case std::ifstream::failbit:
		flag = EFile::FILE_FAILED;
		break;
	case std::ifstream::badbit:
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