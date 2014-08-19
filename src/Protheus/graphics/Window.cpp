#include "Window.h"

namespace Pro{
	Window::Window(const std::string& name, lua_State* L) : CGUID(name)
	{
		lua_state = L;
		windowTitle = name;
		dim.w = 800;
		dim.h = 600;

		createWindow();
	}

	Window::Window()
	{
		windowTitle = "Unnamed";
		dim.w = 800;
		dim.h = 600;

		createWindow();
	}

	Window::~Window()
	{
	}

	void Window::createWindow(){
		window = SDL_CreateWindow(&windowTitle[0], dim.x, dim.y, dim.w, dim.h,
			SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
		if (window == nullptr){
			error.reportFatal("Unable to create window");
			return;
		} 
		luaP_setSDLWindow(lua_state, window); 
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