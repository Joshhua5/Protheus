#include "SpriteBatcher.h"

namespace Pro{
	namespace Graphics {
		SpriteBatcher::SpriteBatcher()
		{
		}


		SpriteBatcher::~SpriteBatcher()
		{
		}

		SpriteBatcher::SpriteBatcher(lua_State* L){
			lua_state = L;
		}

		void SpriteBatcher::push(Asset::Sprite* _s, Math::Vector4& _r){
			sprite_stack.push(_s);
			rect_stack.push(_r);
		}
		void SpriteBatcher::flush(){
			Asset::Sprite* spt;  
			for (int x = sprite_stack.size(); x != 0; x--){
				spt = sprite_stack.top();
				SDL_RenderCopy(renderer, spt->getSpriteSheet(), &spt->getRect().toSDL(), &rect_stack.top().toSDL());
				sprite_stack.pop();
				rect_stack.pop();
			}
		}
	}
}