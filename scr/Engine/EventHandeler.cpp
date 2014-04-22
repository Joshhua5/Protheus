#include "EventHandeler.h"

namespace Pro{

	EventHandeler::EventHandeler()
	{
		input = new Input();
	}


	EventHandeler::~EventHandeler()
	{
		delete input;
	}

	void EventHandeler::update(){
		SDL_Event _event;
		if (SDL_PollEvent(&_event)){
			switch (_event.type){

			case SDL_EventType::SDL_KEYDOWN:
			case SDL_EventType::SDL_KEYUP:
				keyboardEvents.push_back(_event);
				break;

			case SDL_EventType::SDL_MOUSEBUTTONDOWN:
			case SDL_EventType::SDL_MOUSEBUTTONUP:
			case SDL_EventType::SDL_MOUSEMOTION:
			case SDL_EventType::SDL_MOUSEWHEEL:
				mouseEvents.push_back(_event);
				break;
			default:
				systemEvents.push_back(_event);
				break;
			case SDL_EventType::SDL_USEREVENT:
				userEvents.push_back(_event);
				break;
			}
		}
	}
	std::vector<SDL_Event>* EventHandeler::pollKeyEvents(){
		return &keyboardEvents;
	}
	std::vector<SDL_Event>* EventHandeler::pollMouseEvents(){
		return &mouseEvents;
	}
	std::vector<SDL_Event>* EventHandeler::pollSystemEvents(){
		return &systemEvents;
	}
	std::vector<SDL_Event>* EventHandeler::pollUserEvents(){
		return &userEvents;
	}
}