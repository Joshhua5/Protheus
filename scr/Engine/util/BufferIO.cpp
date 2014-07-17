#include "BufferIO.h"

using namespace Pro;
using namespace Util;

// sets the position of the writer
void BufferIO::setPosition(const unsigned position){
	m_head = position;
}
// returns the position of the writer
unsigned BufferIO::getPosition() const{
	return m_head;
}
// skips over the current writing position
void BufferIO::skip(const int position){
	m_head += position;
}
// get's the internal buffers size
unsigned BufferIO::getBufferSize() const{
	return m_buffer->size();
}
unsigned BufferIO::find(const char deliminator) const{
	register auto buf = (char*)m_buffer + m_head;
	while (*buf != deliminator)
		++buf;
	// include the deliminator
	++buf;
	return buf - ((char*)m_buffer + m_head);
}

int BufferIO::lSkip(lua_State* L){
	const auto b = Util::luaP_touserdata<BufferIO>(L, 1);
	b->skip(luaP_toint(L, 2));
	return 0;
}
int BufferIO::lGetPosition(lua_State* L){
	const auto b = Util::luaP_touserdata<BufferIO>(L, 1);
	lua_pushnumber(L, b->getPosition());
	return 1;
}
int BufferIO::lSetPosition(lua_State* L){
	const auto b = Util::luaP_touserdata<BufferIO>(L, 1);
	b->setPosition(luaP_touint(L, 2));
	return 0;
}
int BufferIO::lGetBufferSize(lua_State* L){
	const auto b = Util::luaP_touserdata<BufferIO>(L, 1);
	lua_pushnumber(L, b->getBufferSize());
	return 1;
}