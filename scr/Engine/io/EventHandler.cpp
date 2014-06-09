#include "EventHandler.h"

namespace Pro{

	EventHandler::EventHandler()
	{
		input = new Input();
	}


	EventHandler::~EventHandler()
	{
		delete input;
	}

	void EventHandler::update(){
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
	std::vector<SDL_Event>* EventHandler::pollKeyEvents(){
		return &keyboardEvents;
	}
	std::vector<SDL_Event>* EventHandler::pollMouseEvents(){
		return &mouseEvents;
	}
	std::vector<SDL_Event>* EventHandler::pollSystemEvents(){
		return &systemEvents;
	}
	std::vector<SDL_Event>* EventHandler::pollUserEvents(){
		return &userEvents;
	}


	int EventHandler::lUpdate(lua_State* L){
		EventHandler* e = Util::luaP_touserdata<EventHandler>(L, 1);
		e->update();
		return 0;
	}
}