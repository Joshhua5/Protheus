#include "SpriteBatcher.h"

using namespace Pro;
using namespace Asset;
using namespace Graphics; 
using namespace Math;

SpriteBatcher::SpriteBatcher(lua_State* L){
	renderer = luaP_getSDLRenderer(L);
	lua_state = L;
}

void SpriteBatcher::push(Sprite* _s, Vector4& _r){
	render_stack.push({ _s, _r });
}

void SpriteBatcher::flush(){
	for (auto x = render_stack.size(); x != 0; --x){
		const auto pair = &render_stack.top();
		const auto dstRect = pair->second.toSDL(); 

#ifdef DEBUG
		if (SDL_RenderCopy(renderer, pair->first->getTexture(), NULL, &dstRect) != 0){
			error.reportError("Error drawing to screen"); 
			error.reportError(pair->first->getName() + " : " +  SDL_GetError());
		}
#else
		SDL_RenderCopy(renderer, pair->first->getTexture(), NULL, &dstRect);
#endif 

		render_stack.pop();
	}
}

int SpriteBatcher::lPush(lua_State* L){
	static const auto sb = Util::luaP_touserdata<SpriteBatcher>(L, 1);
	const auto spt = Util::luaP_touserdata<Sprite>(L, 2);

#ifdef DEBUG
	if (sb == nullptr){
		error.reportError("SpriteBatcher called without :");
		return 0;
	}

	if (spt == nullptr){
		error.reportError("Null Sprite Passed");
		return 0;
	}
#endif

	auto v = Math::Vector4(*Util::luaP_touserdata<Vector2>(L, 3), spt->getDimensions());
	sb->push(spt, v); 
	return 0;
}

int SpriteBatcher::lFlush(lua_State* L){
	static const auto sb = Util::luaP_touserdata<SpriteBatcher>(L, 1);
	sb->flush();
	return 0;
}