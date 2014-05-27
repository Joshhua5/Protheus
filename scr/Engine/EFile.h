/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 27:05:2014: Waring J.
*************************************************************************/
#pragma once
namespace Pro{
	namespace Util{
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