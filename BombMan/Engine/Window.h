#pragma once
#include <SDL.h>
#include <string>
namespace Pro{

	class Window
	{
		SDL_Window* window;
		std::string windowTitle; 
		SDL_Rect dim;

	public:
		Window();
		~Window();

		void setTitle(const std::string& title);
		void setWidth(unsigned int);
		void setHeight(unsigned int);
		void setRect(SDL_Rect _rect);
		bool createWindow();
		SDL_Window* getWindow();
	};

}