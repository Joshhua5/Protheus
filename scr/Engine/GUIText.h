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
			GUIText();
			~GUIText();
		};
	}
}
