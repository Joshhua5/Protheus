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
#include "GUIEntity.h"

namespace Pro{
	namespace GUI{
		class GUIText :
			public GUIEntity
		{
		private:
			// content inside the box
			std::string content;
		public:
			GUIText(const std::string& name);
			GUIText();
			~GUIText();

			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "gui_text_metatable";
			}
		};
	}
}
