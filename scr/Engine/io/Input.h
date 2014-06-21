/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	Handles user input
-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <SDL.h>
#include <stack>

using namespace std;

namespace Pro{

	class Input
	{
		stack<SDL_Event> event_stack;
	public:
		Input();
		~Input();

		void update(SDL_Event _event);
		SDL_Event getInputEvent();
		SDL_Event peekInputEvent();
		bool hasEvent();
	};
} 