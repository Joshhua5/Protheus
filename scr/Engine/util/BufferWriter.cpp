#include "BufferWriter.h"

using namespace Pro;
using namespace Util;

BufferWriter::BufferWriter(CBuffer* _buffer){
	m_buffer = _buffer;
	m_head = 0;
}

BufferWriter::~BufferWriter()
{
	m_buffer = nullptr;
}

void BufferWriter::write(void* value, unsigned size){
	if (m_buffer == nullptr)
		return; 
#ifdef DEBUG
	if (m_head + size > m_buffer->size())
		// Resizes the buffer to with 64 bytes overhead
		m_buffer->resize(m_head + size + 64);
#endif
	memcpy(m_buffer->at(m_head) , value, size);
	skip(size);
}


int BufferWriter::lWriteBuffer(lua_State* L){
	const auto b = luaP_touserdata<BufferWriter>(L, 1);
	const auto buf = luaP_touserdata<CBuffer>(L, 2);
	b->write(buf->data(), buf->size());
	return 0;
}

int BufferWriter::lWriteString(lua_State* L){
	const auto b = luaP_touserdata<BufferWriter>(L, 1);
	const auto str = luaP_tostring(L, 2);
	b->write((void*)str.data(), str.length()); 
	b->write('\0');
	return 0;
}

int BufferWriter::lWriteUInt(lua_State* L){
	const auto b = luaP_touserdata<BufferWriter>(L, 1);
	b->write<unsigned>(luaP_touint(L, 2));
	return 0;
}

int BufferWriter::lWriteInt(lua_State* L){
	const auto b = luaP_touserdata<BufferWriter>(L, 1);
	b->write<int>(luaP_toint(L, 2));
	return 0;
}

int BufferWriter::lWriteDouble(lua_State* L){
	const auto b = luaP_touserdata<BufferWriter>(L, 1);
	b->write<double>(lua_tonumber(L, 2));
	return 0;
} 


int BufferWriter::lCreate(lua_State* L){
	Util::luaP_newobject(L,
		new BufferWriter(luaP_touserdata<CBuffer>(L, 1)));
	return 1;
}