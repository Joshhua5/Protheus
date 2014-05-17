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
		};
	}
}
