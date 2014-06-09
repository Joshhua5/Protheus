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
		class GUILabel :
			public GUIEntity
		{
		public:
			GUILabel(const std::string& name);
			GUILabel();
			~GUILabel();

			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "gui_label_metatable";
			}
		};
	}
}
