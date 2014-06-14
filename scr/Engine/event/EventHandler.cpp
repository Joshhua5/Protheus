
#include "EventHandler.h"


namespace Pro{
	using namespace Event;

	EventHandler::EventHandler()
	{
		input = new Input();
	}


	EventHandler::~EventHandler()
	{
		delete input;
	}

	void EventHandler::update() {
		SDL_Event sdl_event;
		CEvent _event;
		IEvent* pro_event;
		if (SDL_PollEvent(&sdl_event)){
			switch (sdl_event.type){

			case SDL_EventType::SDL_KEYDOWN:

				_event.type = EEvent::KEYBOARD;
				pro_event = new KeyboardEvent();
				static_cast<KeyboardEvent*>(pro_event)->key = toKeyboardKey(sdl_event.key.keysym.scancode);
				static_cast<KeyboardEvent*>(pro_event)->key_down = !(static_cast<KeyboardEvent*>(pro_event)->key_up = false);
				_event.event = pro_event;
				keyboardEvents.push_back(_event);
				break;

			case SDL_EventType::SDL_KEYUP:

				_event.type = EEvent::KEYBOARD;
				pro_event = new KeyboardEvent();
				static_cast<KeyboardEvent*>(pro_event)->key = toKeyboardKey(sdl_event.key.keysym.scancode);
				static_cast<KeyboardEvent*>(pro_event)->key_down = !(static_cast<KeyboardEvent*>(pro_event)->key_up = true);
				_event.event = pro_event;
				keyboardEvents.push_back(_event);
				break;

			case SDL_EventType::SDL_MOUSEBUTTONDOWN:

				_event.type = EEvent::MOUSE_BUTTON;
				pro_event = new MouseButton();
				static_cast<MouseButton*>(pro_event)->type = toMouseButton(sdl_event.button);
				static_cast<MouseButton*>(pro_event)->window_position = Math::Vector2(sdl_event.button.x, sdl_event.button.y);
				static_cast<MouseButton*>(pro_event)->button_down = !(static_cast<MouseButton*>(pro_event)->button_up = false);
				_event.event = pro_event;
				mouseEvents.push_back(_event);
				break; 

			case SDL_EventType::SDL_MOUSEBUTTONUP:

				_event.type = EEvent::MOUSE_BUTTON;
				pro_event = new Event::MouseButton();
				static_cast<MouseButton*>(pro_event)->type = toMouseButton(sdl_event.button);
				static_cast<MouseButton*>(pro_event)->window_position = Math::Vector2(sdl_event.button.x, sdl_event.button.y);
				static_cast<MouseButton*>(pro_event)->button_down = !(static_cast<MouseButton*>(pro_event)->button_up = true);
				_event.event = pro_event;
				mouseEvents.push_back(_event);
				break;
				 
			case SDL_EventType::SDL_MOUSEMOTION:

				break;
			case SDL_EventType::SDL_MOUSEWHEEL:
				
				_event.type = EEvent::MOUSE_WHEEL;
				pro_event = new MouseWheel();
				if (sdl_event.wheel.y > 0){
					static_cast<MouseWheel*>(pro_event)->scroll_down = false;
					static_cast<MouseWheel*>(pro_event)->scroll_up = true;
				}else{
					static_cast<MouseWheel*>(pro_event)->scroll_down = true;
					static_cast<MouseWheel*>(pro_event)->scroll_up = false;
				} 
				_event.event = pro_event;
				mouseEvents.push_back(_event);
				break;

			case SDL_EventType::SDL_USEREVENT:
				//userEvents.push_back(_event);
				break;
			default:
				//systemEvents.push_back(_event);
				break;
			}
		}
	}
	std::vector<CEvent>* EventHandler::pollKeyEvents(){
		return &keyboardEvents;
	}
	std::vector<CEvent>* EventHandler::pollMouseEvents(){
		return &mouseEvents;
	}
	std::vector<CEvent>* EventHandler::pollSystemEvents(){
		return &systemEvents;
	}
	std::vector<CEvent>* EventHandler::pollUserEvents(){
		return &userEvents;
	}


	int EventHandler::lUpdate(lua_State* L){
		EventHandler* e = Util::luaP_touserdata<EventHandler>(L, 1);
		e->update();
		return 0;
	}
}