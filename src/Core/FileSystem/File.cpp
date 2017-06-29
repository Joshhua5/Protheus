#include "File.h"

using namespace Pro;
using namespace IO;
using namespace std;

File::File(const std::string& filePath)
{
	m_file_path = filePath;
	file.open(filePath,
		fstream::binary |
		fstream::in |
		fstream::out);

	if (file.is_open() == false)
		global_log.Report<LogCode::FAULT>("Unable to open file: " + filePath, __FUNCTION__, __LINE__);
}

File::~File()
{
	file.close();
}

void File::Open(const std::string& filePath){
	m_file_path = filePath;
	file.open(filePath,
		fstream::binary |
		fstream::in |
		fstream::out);
	if (file.is_open() == false)
		global_log.Report<LogCode::FAULT>("Unable to open file: " + filePath, __FUNCTION__, __LINE__);
}

bool File::IsEndOfFile(){
	if (!file.is_open())
		return true;
	return file.eof();
}

void File::SetReadPosition(unsigned pos){
	if (!file.is_open())
		return;
	file.seekg(pos);
}
unsigned File::GetReadPosition(){
	if (!file.is_open())
		return 0;
	return static_cast<unsigned int>(file.tellg());
}
void File::SetWritePosition(unsigned int pos){
	if (!file.is_open())
		return;
	file.seekp(pos);
}
unsigned int File::GetWritePosition(){
	if (!file.is_open())
		return 0;
	return static_cast<unsigned int>(file.tellp());
}

void File::Write(Buffer& buf){
	if (!file.is_open())
		return;
	file.write(buf.data<const char>(), buf.size());
} 

void File::Write(const std::string& str){
	if (!file.is_open())
		return;
	file.write(str.data(), str.length());
} 

Buffer File::Read(){
	if (!file.is_open())
		return Buffer(0);
	Buffer buf(this->FileSize());
	file.read(buf.data<char>(), buf.size());
	return buf;
}

Buffer File::Read(unsigned size){
	if (!file.is_open())
		Buffer(0);
	Buffer buf(size);
	file.read(buf.data<char>(), buf.size());
	return buf;
}

uint64_t File::ReadBytes(unsigned char amount){
    if(amount > 8)
        return 0;
    // TEST
    uint64_t out = 0;
    file.read((char*)&out, amount);
    return out;
}

std::string File::ReadToken(const char delim){
	string s;
	getline(file, s, delim);
	return s;
}

std::string File::ReadLine(){
	string s;
	getline(file, s, '\n');
	return s;
}

std::string File::ReadString(){
	string s;
	getline(file, s, '\0');
	return s;
}

void File::Close(){
	file.close();
}

bool File::IsOpen(){
	return file.is_open();
}

void File::SkipReadPosition(unsigned amount){
    SetReadPosition(GetReadPosition() + amount);
}

void File::SkipWritePosition(unsigned amount){
    SetWritePosition(GetWritePosition() + amount);
}

unsigned int File::FileSize(){
	if (!IsOpen())
		return 0;
	
	const auto current_position = GetReadPosition();
	file.seekg(0, std::ios::end);
	auto ending_position = GetReadPosition();
	SetReadPosition(current_position);
	return ending_position;
}

EFile File::PeekError(){
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

EFile File::GetError(){
	EFile f = PeekError();
	flag = EFile::NO_FILE_ERROR;
	return f;
}