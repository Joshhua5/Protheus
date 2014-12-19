/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
A structure to hold a pointer to a buffer, also stores the size of this
buffer. The constructor will define the size in bytes and allocate the
buffer if a size is provided.

-------------------------------------------------------------------------
History:
- 1:06:2014: Waring J.
*************************************************************************/
#pragma once

#include "BufferBase.h"  

class CBuffer :
	protected BufferBase
{
public:
	// if copy is true then the data is copied otherwise
	// the data is moved, you can't delete the pointer
	// which was used to pass data otherwise you'll
	// delete the internal data aswell and visversa.
   


	inline  CBuffer(const CBuffer& b, bool copy) {
		init(b.data(), b.size(), copy);
	}
	inline  CBuffer(void* _data, const unsigned _size, const bool copy) {
		if (_size == 0) {
			m_data = nullptr;
			m_size = 0;
			return;
		}

		if (copy) {
			m_data = new char[_size];
			memcpy(m_data, _data, _size);
		}
		else
			m_data = _data;

		wasCopied = copy;
		m_size = _size;
	}		    
	inline  CBuffer(const unsigned _size)
	{
		if (_size == 0)
			m_data = nullptr;
		else
			m_data = new char[_size];
		wasCopied = false;
		m_size = _size;
	}		    
	inline  CBuffer()
	{
		wasCopied = false;
		m_size = 0;
		m_data = nullptr;
	}		    
	inline  CBuffer(CBuffer&& b) {
		m_data = b.data();
		m_size = b.size();
		wasCopied = b.wasCopied;
		b.dereference();
	}

	inline ~CBuffer()
	{
		if (wasCopied && m_size != 0)
			delete[] m_data;
		m_data = nullptr;
	}
	inline CBuffer&  operator=(CBuffer&& b) {
		if (this == &b)
			return *this;

		m_data = b.data();
		m_size = b.size();
		wasCopied = b.wasCopied;
		b.dereference();
		return *this;
	}
					 
					 
	inline CBuffer&  operator=(const CBuffer& b) {

		if (this == &b)
			return *this;

		this->init(b.data<void>(), b.size(), true);
		return *this;
	}


	inline void  init(const void* _data, const unsigned _size, const bool copy) {
		// Check if data has been initialized
		if (m_data != nullptr)
			delete[] m_data;

		if (copy) {
			m_data = new char[_size];
			memcpy(m_data, _data, _size);
		}
		else
			m_data = const_cast<void*>(_data);

		wasCopied = copy;
		m_size = _size;
	}
				 
	inline void  init(const unsigned _size) {
		init(new char[_size], _size, false);
	}
				 
	inline void  dereference() {
		m_data = nullptr;
		m_size = 0;
	}

	template<typename T>
	inline T* data() const {
		return static_cast<T*>(m_data);
	}

	inline void* data() const {
		return m_data;
	}

	inline unsigned  size() const {
		return m_size;
	}

	inline bool isEmpty() const {
		return (m_size == 0) ? true : false;
	}

	inline void resize(const unsigned size) {
		const auto old_data = m_data;
		m_data = new char[size];

		if (m_size > size)
			// Reducing
			memcpy(m_data, old_data, size);
		else
			// Increasing
			memcpy(m_data, old_data, m_size);

		m_size = size;
		delete[] old_data;
	}
	template<typename T>
	inline unsigned count(const T& value) {
		T* buffer = data<T>();
		unsigned count = 0;
		for (unsigned head = 0; head < (size() / sizeof(T)); ++head, ++buffer)
			if (*buffer == value)
				++count;
		return count;
	}
	inline void* at(const unsigned pos) const {
		if (pos < m_size)
			return reinterpret_cast<char*>(m_data) + pos;
		return nullptr;
	}
	inline void* operator[](const unsigned pos) const {
		return reinterpret_cast<char*>(m_data) + pos;
	}
};

