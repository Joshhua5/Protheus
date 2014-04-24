#pragma once
#include "GUIEntity.h"

namespace Pro{
	namespace GUI{
		enum struct GUI_EVENT{
			FOCUS,
			CLICK
		};
		enum struct GUI_ENTITY_TYPE{
			BUTTON,
			LABEL,
			SLIDER,
			TEXT,
			WINDOW,
			COLLAPSIBLE_MENU
		};
		struct GUIEvent
		{
			GUI_EVENT event_type;
			GUI_ENTITY_TYPE entity_type;
			void* entity_pointer;
		};
	}
	}