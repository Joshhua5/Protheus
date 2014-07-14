/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	abstract class for buffers
-------------------------------------------------------------------------
History:
- 13:07:2014: Waring J.
*************************************************************************/
#pragma once

#include <memory>

class BufferBase
{
protected:
	unsigned int m_size;
	void* m_data;
public: 

	virtual void* data() = 0;
	virtual void dereference() = 0;

	virtual void* at(unsigned) = 0;
	virtual void* operator[](unsigned) = 0; 
	virtual unsigned size() = 0;
};
