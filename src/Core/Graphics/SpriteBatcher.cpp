#include "SpriteBatcher.h"

using namespace Pro;
using namespace Asset;
using namespace Graphics; 
using namespace Math; 

SpriteBatcher::SpriteBatcher(SDL_Renderer* _renderer){
	renderer = _renderer;
}
 
void SpriteBatcher::push(Sprite* _s, Vector4& _r, float scale, float rotation){ 
	Details details;
	details.position = _r;
	details.scale = scale;
	details.sprite = _s;
	details.rotation = rotation;
	render_stack.push(details);
}
 
void SpriteBatcher::flush(){
	for (auto x = render_stack.size(); x != 0; --x){
		auto details = render_stack.top(); 
		auto dstRect = SDL_RectCreate(details.position); 
		auto center = *details.sprite->getCenter();

		// Scale
		dstRect.w = (int)((float)dstRect.w * details.scale);
		dstRect.h = (int)((float)dstRect.h * details.scale); 
		// Scale the center as well
		center.x = (int) ((float) center.x * details.scale);
		center.y = (int) ((float) center.y * details.scale);

		if (SDL_RenderCopyEx(
			renderer,
			details.sprite->getTexture(),
			NULL,
			&dstRect,
			details.rotation,
			&center,
			SDL_FLIP_NONE) != 0) 
			error.reportError("Error during SpriteBatcher Flush: " + string(SDL_GetError())); 

		render_stack.pop(); 
	}
}
