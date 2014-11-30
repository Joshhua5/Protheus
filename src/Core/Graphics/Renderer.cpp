#include "Renderer.h"

using namespace Pro;
using namespace Graphics;
using namespace Asset;
  
Renderer::Renderer(const Window& _window){
	window = _window.getWindow();  
}
 

Renderer::~Renderer()
{ 
}

void Renderer::setWindow(const Window& _window) {
	window = _window.getWindow();
}

void Renderer::startFrame(){
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT |
		GL_ACCUM_BUFFER_BIT);
}

void Renderer::endFrame(){ 
	glfwSwapBuffers(window);
}
