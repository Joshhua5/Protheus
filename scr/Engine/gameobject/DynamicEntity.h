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
		class DynamicEntity :
			Position,
			Area,
			ActiveState,
			CScriptable,
			Textured,
			Animated,
			CGUID

		{
		public:
			DynamicEntity();
			~DynamicEntity();
		};
	}
}


