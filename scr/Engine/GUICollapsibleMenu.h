/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once
#include <SDL_events.h>
#include <vector>
#include "GUIEntity.h" 
#include "GUIContainer.h"
#include "GUIDLookup.h"

namespace Pro{
	namespace GUI{
		class GUICollapsibleMenu : 
			public GUIContainer
		{
		public:
			GUICollapsibleMenu(const std::string& name);
			GUICollapsibleMenu();
			~GUICollapsibleMenu(); 
		};
	}
}
