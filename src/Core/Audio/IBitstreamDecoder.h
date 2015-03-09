/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
-------------------------------------------------------------------------

Description:
A interface for the implementation of different track loads which
process different types of file types and compression algorithms

-------------------------------------------------------------------------
History:
- 09:06:2014 Waring J.
*************************************************************************/
#pragma once

#include "CAudioData.h"


namespace Pro{
	namespace Audio{
		class IBitstreamDecoder
		{
		public: 
			virtual CAudioData* load(Buffer& buffer) = 0;
		};
	}
}
