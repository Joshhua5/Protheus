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
	unsigned int m_size;
	void* m_data;
	bool wasCopied;
};
