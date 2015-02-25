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
		global_log.Report<LogCode::ERROR>("Unable to open file: " + filePath, __FUNCTION__, __LINE__);
}

CFile::~CFile()
{
	file.close();
}

void CFile::Open(const std::string& filePath){
	m_file_path = filePath;
	file.open(filePath,
		fstream::binary |
		fstream::in |
		fstream::out);
	if (file.is_open() == false)
		global_log.Report<LogCode::ERROR>("Unable to open file: " + filePath, __FUNCTION__, __LINE__);
}

bool CFile::IsEndOfFile(){
	if (!file.is_open())
		return true;
	return file.eof();
}

void CFile::SetReadPosition(unsigned int pos){
	if (!file.is_open())
		return;
	file.seekg(pos);
}
unsigned int CFile::GetReadPosition(){
	if (!file.is_open())
		return 0;
	return static_cast<unsigned int>(file.tellg());
}
void CFile::SetWritePosition(unsigned int pos){
	if (!file.is_open())
		return;
	file.seekp(pos);
}
unsigned int CFile::GetWritePosition(){
	if (!file.is_open())
		return 0;
	return static_cast<unsigned int>(file.tellp());
}

void CFile::Write(Buffer& buf){
	if (!file.is_open())
		return;
	file.write(buf.data<const char>(), buf.size());
} 

void CFile::Write(const std::string& str){
	if (!file.is_open())
		return;
	file.write(str.data(), str.length());
} 

Buffer CFile::Read(){
	if (!file.is_open())
		return Buffer(0);
	Buffer buf(this->FileSize());
	file.read(buf.data<char>(), buf.size());
	return buf;
}

Buffer CFile::Read(unsigned size){
	if (!file.is_open())
		Buffer(0);
	Buffer buf(size);
	file.read(buf.data<char>(), buf.size());
	return buf;
} 

std::string CFile::ReadToken(const char delim){
	string s;
	getline(file, s, delim);
	return s;
}

std::string CFile::ReadLine(){
	string s;
	getline(file, s, '\n');
	return s;
}

std::string CFile::ReadString(){
	string s;
	getline(file, s, '\0');
	return s;
}

void CFile::Close(){
	file.close();
}

bool CFile::IsOpen(){
	return file.is_open();
}

unsigned int CFile::FileSize(){
	if (!IsOpen())
		return 0;
	
	const auto current_position = GetReadPosition();
	file.seekg(0, std::ios::end);
	auto ending_position = GetReadPosition();
	SetReadPosition(current_position);
	return ending_position;
}

EFile CFile::PeekError(){
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

EFile CFile::GetError(){
	EFile f = PeekError();
	flag = EFile::NO_FILE_ERROR;
	return f;
}