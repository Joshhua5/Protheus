/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	abstract extern class for buffers
-------------------------------------------------------------------------
History:
- 13:07:2014: Waring J.
*************************************************************************/
#pragma once

#include <memory> 
#include "Error.h" 

class BufferBase
{
protected:
	/*! Remembers if the buffer was copied to prevent the deconstructor from deleting data which it doesn't own */
	bool wasCopied;
	/*! Size of the buffer in bytes */
	unsigned int m_size;
	/*! Pointer to the data in the heap*/
	void* m_data;
};
