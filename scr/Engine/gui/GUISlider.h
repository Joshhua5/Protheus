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
		class GUISlider :
			public GUIEntity
		{
		private:
			// the lowest value of the slider
			double lowValue;
			// the highest value of the slider
			double highValue;
			// the current value of the slider
			double value;
		public:
			GUISlider(const std::string& name);
			GUISlider();
			~GUISlider();

			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "gui_slider_metatable";
			}
		};
	}
}