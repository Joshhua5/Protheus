#include "BufferReader.h"

using namespace Pro;
using namespace Util;

BufferReader::BufferReader(CBuffer* _buffer){
	m_buffer = _buffer;
}

BufferReader::~BufferReader(){
	m_buffer = nullptr;
}

inline char* BufferReader::read_raw() const{
	return m_buffer->data<char>() + m_head;
}

CBuffer BufferReader::read(const int size){ 
	CBuffer out(read_raw(), size, true);
	skip(size);
	return move(out);
}

CBuffer BufferReader::read_delim(const char deliminator){
	return read(find(deliminator));
}

// reads until a null terminator is found '\0'
CBuffer BufferReader::read_string(){
	return read_delim('\0');
}

int BufferReader::lReadString(lua_State* L){
	const auto b = luaP_touserdata<BufferReader>(L, 1); 
	lua_pushstring(L, b->read_string().data<char>());
	return 1;
}

int BufferReader::lReadUInt(lua_State* L){
	const auto b = luaP_touserdata<BufferReader>(L, 1);
	lua_pushnumber(L, b->read<unsigned>());
	return 1;
}

int BufferReader::lReadInt(lua_State* L){
	const auto b = luaP_touserdata<BufferReader>(L, 1);
	lua_pushnumber(L, b->read<int>());
	return 1;
}


int BufferReader::lReadDouble(lua_State* L){
	const auto b = luaP_touserdata<BufferReader>(L, 1);
	lua_pushnumber(L, b->read<double>());
	return 1;
}

int BufferReader::lCreate(lua_State* L){
	Util::luaP_newobject(L,
		new BufferReader(luaP_touserdata<CBuffer>(L, 1)));
	return 1;
}