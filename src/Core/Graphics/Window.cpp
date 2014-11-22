#include "Window.h"

using namespace Pro;
using namespace Graphics;

Window::Window(const std::string& name)
{
	windowTitle = name;
	dim.w = 800;
	dim.h = 600;

	window = SDL_CreateWindow(&windowTitle[0], dim.x, dim.y, dim.w, dim.h,
		SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);

	if (window == nullptr)
		error.reportFatal("Unable to create window");
}

Window::~Window()
{
}

SDL_Window* Window::getWindow() const {
	return window;
}

void Window::setTitle(const std::string& title) {
	windowTitle = title;
}
void Window::setWidth(unsigned int _width) {
	dim.w = _width;
}
void Window::setHeight(unsigned int _height) {
	dim.h = _height;
}
void Window::setRect(SDL_Rect _rect) {
	dim = _rect;
}