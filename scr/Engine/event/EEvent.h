/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 10:06:2014: Waring J.
*************************************************************************/
#pragma once 
namespace Pro{
	namespace Event{
		enum struct EEvent{
			KEYBOARD, //
			TEXT_INPUT,
			TEXT_EDIT,
			MOUSE_BUTTON,
			MOUSE_MOTION, //
			MOUSE_WHEEL,
			SYSTEM,
			QUIT,
			CONTROLLER_AXIS,
			CONTROLLER_BUTTON,
			CONTROLLER_DEVICE,
			TOUCH,
			MULTI_TOUCH,
			WINDOW,
			CUSTOM,
			DROP_EVENT
		};
	}
}
