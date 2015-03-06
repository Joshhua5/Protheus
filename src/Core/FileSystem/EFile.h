/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
-------------------------------------------------------------------------

Description:
Enumeration of file return and state types
-------------------------------------------------------------------------
History:
- 27:05:2014: Waring J.
*************************************************************************/
#pragma once

namespace Pro{
	namespace IO{
		enum struct EFile{
			FILE_BUFFER_OVERFLOW,
			FILE_NOT_FOUND,
			FILE_INVALID_DATA,
			FILE_CLOSED,
			FILE_FAILED,
			FILE_INVALID_OPERATION,
			END_OF_FILE,
			NO_ERROR
		};
	}
}