#include "SpriteBatcher.h"

namespace Pro{
	namespace Graphics {
		SpriteBatcher::SpriteBatcher()
		{
		}


		SpriteBatcher::~SpriteBatcher()
		{
		}

		SpriteBatcher::SpriteBatcher(SDL_Renderer* _r){
			renderer = _r; 
		}

		void SpriteBatcher::push(Sprite* _s, SDL_Rect _r){
			sprite_stack.push(_s);
			rect_stack.push(_r);
		}
		void SpriteBatcher::flush(){
			Sprite* spt; 
			for (int x = sprite_stack.size(); x != 0; x--){
				spt = sprite_stack.top();
				SDL_RenderCopy(renderer, spt->getSpriteSheet(), &spt->getRect(), &rect_stack.top());
				sprite_stack.pop();
				rect_stack.pop();
			}
		}
	}
}