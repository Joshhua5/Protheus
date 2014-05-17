#pragma once
#include <vector>
#include "Input.h" 

namespace Pro{

	class EventHandeler
	{
	private:
		Input* input;
		std::vector<SDL_Event> keyboardEvents;
		std::vector<SDL_Event> mouseEvents;
		std::vector<SDL_Event> systemEvents;
		std::vector<SDL_Event> userEvents;
	public:
		EventHandeler();
		~EventHandeler();

		void update();
		std::vector<SDL_Event>* EventHandeler::pollKeyEvents();
		std::vector<SDL_Event>* EventHandeler::pollMouseEvents(); 
		std::vector<SDL_Event>* EventHandeler::pollSystemEvents();
		std::vector<SDL_Event>* EventHandeler::pollUserEvents(); 
	}; 
}