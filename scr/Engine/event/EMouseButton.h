/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	Stores which button was pressed in a Mouse event
-------------------------------------------------------------------------
History:
- 10:06:2014: Waring J.
*************************************************************************/
#pragma once 
namespace Pro{
	namespace Event{
		enum struct EMouseButton{
			UNKNOWN = 0,
			LEFT,
			MIDDLE,
			RIGHT
		};

		EMouseButton inline toMouseButton(SDL_MouseButtonEvent& _event){
			EMouseButton out;
			out = static_cast<EMouseButton>(_event.button);
			return out;
		}
	}
}
