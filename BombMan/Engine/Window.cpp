#include "Window.h"

namespace Pro{

	Window::Window()
	{
		windowTitle = "Core";
		dim.w = 800;
		dim.h = 600;
	}


	Window::~Window()
	{
	}

	bool Window::createWindow(){
		window = SDL_CreateWindow(windowTitle.c_str(), dim.x, dim.y, dim.w, dim.h,
			SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL);
		if (window == nullptr)
			return false;
		return true;
	}

	SDL_Window* Window::getWindow(){
		return window;
	}

	void Window::setTitle(const std::string& title){
		windowTitle = title;
	}
	void Window::setWidth(unsigned int _width){
		dim.w = _width;
	}
	void Window::setHeight(unsigned int _height){
		dim.h = _height;
	}
	void Window::setRect(SDL_Rect _rect){
		dim = _rect;
	}
}