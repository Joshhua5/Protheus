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

#include "GUIEntity.h"
namespace Pro{
	namespace GUI{
		class GUIButton :
			 public GUIEntity
		{
		public: 
			GUIButton(const std::string& name);
			GUIButton();
			~GUIButton();

			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "gui_button_metatable";
			}
		};
	}
}
