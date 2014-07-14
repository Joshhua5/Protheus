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
	m_size = _size;
	m_data = new char[_size];
}

CBuffer::CBuffer()
{
	m_size = 0;
	m_data = nullptr;
}

CBuffer::~CBuffer()
{
	delete [] m_data;
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

void* CBuffer::at(const unsigned pos) const {
	if (pos < m_size)
		return reinterpret_cast<char*>(m_data) + pos;
	return nullptr;
}
void* CBuffer::operator[](const unsigned pos) const{
	return reinterpret_cast<char*>(m_data) + pos;
}
