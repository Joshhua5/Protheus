
#include "Input.h"

namespace Pro{

	Input::Input()
	{
	}


	Input::~Input()
	{
	}

	void Input::update(SDL_Event _event){
		event_stack.push(_event);
	}
	SDL_Event Input::getInputEvent(){
		SDL_Event event = event_stack.top();
		event_stack.pop();
		return event;
	}
	SDL_Event Input::peekInputEvent(){
		return event_stack.top();
	}

	bool Input::hasEvent(){
		return !event_stack.empty();
	}
}