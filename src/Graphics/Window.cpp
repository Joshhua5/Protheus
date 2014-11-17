#include "Window.h"
#include "glcorearb.h"

using namespace Pro;
using namespace Math;

Window::Window()
{

}


Window::~Window()
{

}

void Window::makeCurrent(){ 
#if PRO_OPENGL
	wglMakeCurrent(hDC, context); 
#endif
}

void Window::renderFrame(){
#if PRO_OPENGL
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers
	SwapBuffers(hDC); // Swap buffers so we can see our rendering
#endif
}

void Window::setViewport(Vector2& xy, Vector2& dim){
#if PRO_OPENGL
	glViewport(xy.x, xy.y, dim.x, dim.y); // Set the viewport size to fill the window
#endif
}