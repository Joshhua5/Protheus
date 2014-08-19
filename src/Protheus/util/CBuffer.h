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
#include "LuaUtils.h"

class CBuffer :
	protected BufferBase
{
public:
	// if copy is true then the data is copied otherwise
	// the data is moved, you can't delete the pointer
	// which was used to pass data otherwise you'll
	// delete the internal data aswell.
	CBuffer(void* data, const unsigned size, const bool copy);
	CBuffer(const unsigned size);
	CBuffer();
	// Move
	CBuffer(CBuffer&&);
	CBuffer& operator=(CBuffer&&);
	// Copy
	CBuffer(const CBuffer&);
	CBuffer& operator=(const CBuffer&);
	~CBuffer();

	

	void init(void* data, const unsigned size, const bool copy);
	void init(const unsigned size);

	template<typename T> T* data() const{
		return reinterpret_cast<T*>(m_data);
	}

	void resize(const unsigned);

	void* data() const;
	unsigned size() const;

	void* at(const unsigned pos) const;
	void* operator[](const unsigned pos) const;
	 
	// removes the internet reference
	// to prevent the data from being deleted
	// when CBuffer is deleted.
	// don't dereference unless the data pointer
	// has been stored somewhere else (memory leak)
	void dereference();
	 
	static int lSize(lua_State*);
	static int lResize(lua_State*);
	static int lCreate(lua_State*); 

	/*constexpr*/ static const char* lGetMetatable(){
		return "buffer_metatable";
	}

	template<typename T>
	static inline void lGetFunctions(std::vector<luaL_Reg>& fields){ 
		fields.push_back({ "size", &T::lSize });
		fields.push_back({ "resize", &T::lResize });
	}
};
