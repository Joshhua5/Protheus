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

#include <string>
#include "GUIContainer.h"

namespace Pro{
	namespace GUI{
		class GUIDropDownMenu :
			public GUIContainer
		{
		public:
			GUIDropDownMenu(const std::string& name);
			GUIDropDownMenu(); 

			// returns the Metatable's name assosiated with this object
			constexpr static const char* lGetMetatable(){
				return "gui_dropdown_menu_metatable";
			}
			 
			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				GUIContainer::lGetFunctions<T>(fields);
			}
		};
	}
}
