#include "CBuffer.h"

/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 14:07:2014: Waring J.
*************************************************************************/
#pragma once

#include "BufferBase.h" 


CBuffer::CBuffer(void* _data, const unsigned _size,const bool copy){
	if (_size == 0){
		m_data = nullptr;
		m_size = 0;
		return;
	}

	if (copy){
		m_data = new char[_size];
		memcpy(m_data, _data, _size);
	}
	else
		m_data = _data;

	m_size = _size;
}

CBuffer::CBuffer(const unsigned _size)
{
	if (_size == 0)
		m_data = nullptr; 
	else
		m_data = new char[_size];
	m_size = _size; 
}

CBuffer::CBuffer()
{
	m_size = 0;
	m_data = nullptr;
}

CBuffer::~CBuffer()
{
	delete [] m_data;
	m_data = nullptr;
}

void CBuffer::init(void* _data,const unsigned _size,const bool copy){
	// Check if data has been initialized
	if (m_data != nullptr)
		delete [] m_data;

	if (copy){
		m_data = new char[_size];
		memcpy(m_data, _data, _size);
	}
	else
		m_data = _data;

	m_size = _size;
}

void CBuffer::init(const unsigned _size){
	init(new char[_size], _size, false);
}

void CBuffer::dereference(){
	m_data = nullptr;
	m_size = 0;
}

void* CBuffer::data() const{
	return m_data;
}
unsigned CBuffer::size() const{
	return m_size;
}

void CBuffer::resize(const unsigned size){
	const auto old_data = m_data;
	m_data = new char[size];
	if (m_size > size)
		// Reducing
		memcpy(m_data, old_data, size);
	else 
		// Increasing
		memcpy(m_data, old_data, m_size);
	m_size = size;
	delete [] m_data;
}

void* CBuffer::at(const unsigned pos) const {
	if (pos < m_size)
		return reinterpret_cast<char*>(m_data) + pos;
	return nullptr;
}
void* CBuffer::operator[](const unsigned pos) const{ 
	return reinterpret_cast<char*>(m_data)+pos;
}
 
int  CBuffer::lSize(lua_State* L){
	const auto b = Pro::Util::luaP_touserdata<CBuffer>(L, 1);
	luaP_pushnumber(L, b->size());
	return 1;
}

int  CBuffer::lResize(lua_State* L){
	const auto b = Pro::Util::luaP_touserdata<CBuffer>(L, 1);
	b->resize(luaP_touint(L, 1)); 
	return 0;
}

int CBuffer::lCreate(lua_State* L){
	Pro::Util::luaP_newobject(L, new CBuffer(luaP_touint(L, 1)));
	return 1;
}