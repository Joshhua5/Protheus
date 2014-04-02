#pragma once
#include <SDL_events.h>
#include <stack>
namespace Pro{

	class Input
	{
		std::stack<SDL_Event> event_stack;
	public:
		Input();
		~Input();

		void update(SDL_Event _event);
		SDL_Event getInputEvent();
		SDL_Event peekInputEvent();
		bool hasEvent();
	};
}