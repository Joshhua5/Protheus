#include "Renderer.h"

using namespace Pro;
using namespace Graphics;
using namespace Asset;
  
Renderer::Renderer(const Window& window){ 
	SDL_Window* w = window.getWindow();
	
	renderer = SDL_CreateRenderer(w, -1,
		SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr){
		error.reportFatal("Failure to create Renderer");
		return;
	}  
	 
}

Renderer::~Renderer()
{ 
}

SDL_Renderer* Renderer::getRenderer(){
	return renderer;
}
 
void Renderer::startFrame(){
	SDL_RenderClear(renderer);
}

void Renderer::endFrame(){ 
	SDL_RenderPresent(renderer);
}
