/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 10:06:2014: Waring J.
*************************************************************************/
#pragma once 

#include "EEvent.h" 
#include "IEvent.h"

namespace Pro{
	namespace Event{
		struct CEvent{
			EEvent type;
			// use the type to cast the event and retrieve it's data
			IEvent* event; 
		};
	} 
}
