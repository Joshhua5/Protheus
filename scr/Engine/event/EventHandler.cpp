#include "EventHandler.h"

using namespace Pro;
using namespace Event;
using namespace Util;
using namespace Math;
using namespace std;

void EventHandler::update() {
	SDL_Event sdl_event;

	MouseButton mbEvent;
	MouseMotion mmEvent;
	KeyboardEvent kEvent;
	MouseWheel mwEvent;

	while (SDL_PollEvent(&sdl_event)){
		switch (sdl_event.type){
		case SDL_EventType::SDL_KEYDOWN:

			kEvent.key = 
				toKeyboardKey(sdl_event.key.keysym.scancode);
			kEvent.key_down = true;
			kEvent.key_up = false;
			keyEvent.push(kEvent);

			break;

		case SDL_EventType::SDL_KEYUP:
			
			kEvent.key = 
				toKeyboardKey(sdl_event.key.keysym.scancode); 
			kEvent.key_down = false;
			kEvent.key_up = true;
			keyEvent.push(kEvent);

			break;

		case SDL_EventType::SDL_MOUSEBUTTONDOWN:

			mbEvent.type = toMouseButton(sdl_event.button);
			mbEvent.window_position = Vector2(sdl_event.button.x, sdl_event.button.y);
			mbEvent.button_down = true;
			mbEvent.button_up = false;

			mouseButton.push(mbEvent);
			break;

		case SDL_EventType::SDL_MOUSEBUTTONUP:

			mbEvent.type = toMouseButton(sdl_event.button);
			mbEvent.window_position = Vector2(sdl_event.button.x, sdl_event.button.y);
			mbEvent.button_down = false;
			mbEvent.button_up = true;

			mouseButton.push(mbEvent);
			break;

		case SDL_EventType::SDL_MOUSEMOTION:

			mmEvent.window_position = Vector2(sdl_event.motion.x, sdl_event.motion.y);
			mmEvent.relative_position = Vector2(sdl_event.motion.yrel, sdl_event.motion.yrel);
			mouseMotion.push(mmEvent);

			break;
		case SDL_EventType::SDL_MOUSEWHEEL:

			if (sdl_event.wheel.y > 0){
				mwEvent.scroll_down = false;
				mwEvent.scroll_up = true;
			}
			else{
				mwEvent.scroll_down = true;
				mwEvent.scroll_up = false;
			}

			mouseWheel.push(mwEvent);
			break;

		case SDL_EventType::SDL_USEREVENT:
			//userEvents.push_back(_event);
			break;
		//default:
			//systemEvents.push_back(_event);
		//	break;
		}
	}
}

stack<KeyboardEvent>* EventHandler::pollKey(){
	return &keyEvent;
}
stack<MouseButton>* EventHandler::pollMouseButton(){
	return &mouseButton;
}
stack<MouseMotion>* EventHandler::pollMouseMotion(){
	return &mouseMotion;
} 

stack<MouseWheel>* EventHandler::pollMouseWheel(){
	return &mouseWheel;
}

int EventHandler::lKey(lua_State* L){ 
	const auto e = lua_tostring(L, 1)[0];
	lua_pushnumber(L, e);
	return 1;
}

int EventHandler::lGetMouseMotion(lua_State* L){
	static const auto eh = luaP_touserdata<EventHandler>(L, 1);
	static const auto e = eh->pollMouseMotion();
	if (e->size() == 0)
		return 0;
	luaP_newobject<MouseMotion>(L, e->top());
	e->pop();
	return 1;
}
int EventHandler::lGetMouseMotionCount(lua_State* L){
	static const auto eh = luaP_touserdata<EventHandler>(L, 1);
	static const auto e = eh->pollMouseMotion();
	lua_pushnumber(L, e->size());
	return 1;
}

int EventHandler::lGetMouseMotionHasNext(lua_State* L){
	static const auto eh = luaP_touserdata<EventHandler>(L, 1);
	static const auto e = eh->pollMouseMotion();
	lua_pushboolean(L, !(e->empty()));
	return 1;
}

int EventHandler::lGetKey(lua_State* L){
	static const auto eh = luaP_touserdata<EventHandler>(L, 1);
	static const auto e = eh->pollKey();
	if (e->size() == 0)
		return 0;
	luaP_newobject<KeyboardEvent>(L, e->top());
	e->pop();
	return 1;
}

int EventHandler::lGetKeyHasNext(lua_State* L){
	static const auto eh = luaP_touserdata<EventHandler>(L, 1);
	static const auto e = eh->pollKey();
	lua_pushboolean(L, !(e->empty()));
	return 1;
}
int EventHandler::lGetKeyCount(lua_State* L){
	static const auto eh = luaP_touserdata<EventHandler>(L, 1);
	static const auto e = eh->pollKey();
	lua_pushnumber(L, e->size());
	return 1;
}

int EventHandler::lGetMouseButton(lua_State* L){
	static const auto eh = luaP_touserdata<EventHandler>(L, 1);
	static const auto e = eh->pollMouseButton();
	if (e->size() == 0)
		return 0;
	luaP_newobject<MouseButton>(L, e->top());
	e->pop();
	return 1;
}

int EventHandler::lGetMouseButtonCount(lua_State* L){
	static const auto eh = luaP_touserdata<EventHandler>(L, 1);
	static const auto e = eh->pollMouseButton();
	lua_pushboolean(L, !(e->empty()));
	return 1;
}

int EventHandler::lGetMouseButtonHasNext(lua_State* L){
	static const auto eh = luaP_touserdata<EventHandler>(L, 1);
	static const auto e = eh->pollMouseButton();
	lua_pushnumber(L, e->size());
	return 1;
}

int EventHandler::lGetMouseWheel(lua_State* L){
	static const auto eh = luaP_touserdata<EventHandler>(L, 1);
	static const auto e = eh->pollMouseWheel();
	if (e->size() == 0)
		return 0;
	luaP_newobject<MouseWheel>(L, e->top());
	e->pop();
	return 1;
}

int EventHandler::lGetMouseWheelCount(lua_State* L){
	static const auto eh = luaP_touserdata<EventHandler>(L, 1);
	static const auto e = eh->pollMouseWheel();
	lua_pushboolean(L, !(e->empty()));
	return 1;
}

int EventHandler::lGetMouseWheelHasNext(lua_State* L){
	static const auto eh = luaP_touserdata<EventHandler>(L, 1);
	static const auto e = eh->pollMouseWheel();
	lua_pushnumber(L, e->size());
	return 1;
}