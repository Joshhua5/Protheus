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
			GUISlider();
			~GUISlider();
		};
	}
}