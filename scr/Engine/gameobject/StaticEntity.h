/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	 
-------------------------------------------------------------------------
History:
- 25:06:2014: Waring J.
*************************************************************************/
#pragma once

#include "..\Components.h"

namespace Pro{
	namespace GameObject{
		using namespace Component;
		class StaticEntity : 
			Position,
			Area,
			Textured,
			Animated,
			CGUID
		{
		public:
			StaticEntity();
			~StaticEntity();
		}; 
	}
}

