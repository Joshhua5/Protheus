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

#include <vector>
#include "GUIEntity.h"
#include "GUIContainer.h"
#include "..\GUIDLookup.h"

namespace Pro{
	namespace GUI{
		class GUICollapsibleMenu :
			public GUIContainer
		{
		public:
			GUICollapsibleMenu(const std::string& name);
			GUICollapsibleMenu(); 

			// returns the Metatable's name assosiated with this object
			/*constexpr*/ static const char* lGetMetatable(){
				return "gui_collapsible_menu_metatable";
			}


			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				GUIContainer::lGetFunctions<T>(fields);
			}
		};
	}
}
