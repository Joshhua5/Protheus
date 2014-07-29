#include "SpriteBatcher.h"

using namespace Pro;
using namespace Asset;
using namespace Graphics; 
using namespace Math;
using namespace Util;

SpriteBatcher::SpriteBatcher(lua_State* L){
	renderer = luaP_getSDLRenderer(L);
	lua_state = L;
}

void SpriteBatcher::push(Sprite* _s, Vector4& _r){
	push(_s, _r, 1.0f, 0.0f); 
}
void SpriteBatcher::push(Sprite* _s, Vector4& _r, float scale){
	push(_s, _r, scale, 0.0f);
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
		dstRect.w = (int)((float)dstRect.w *details.scale);
		dstRect.h = (int)((float)dstRect.h * details.scale); 
		// Scale the center as well
		center.x = (int) ((float) center.x * details.scale);
		center.y = (int) ((float) center.y * details.scale);

#ifdef DEBUG  
		if (SDL_RenderCopyEx(
			renderer,
			details.sprite->getTexture(),
			NULL,
			&dstRect,
			details.rotation,
			&center,
			SDL_FLIP_NONE) != 0){

			error.reportError("Error drawing to screen"); 
			error.reportError(details.sprite->getName() + " : " + SDL_GetError());
		}
#else
		SDL_RenderCopyEx(
			renderer,
			details.sprite.getTexture(),
			NULL,
			&dstRect,
			details.rotation,
			details.sprite.getCenter(),
			SDL_FLIP_NONE)
#endif 

		render_stack.pop(); 
	}
}

int SpriteBatcher::lPush(lua_State* L){
	static const auto sb = Util::luaP_touserdata<SpriteBatcher>(L, 1);
	auto spt = Util::luaP_touserdata<Sprite>(L, 2); 
	auto v = Vector4(*luaP_touserdata<Vector2>(L, 3), spt->getDimensions());
	// - 1 because of the automatically passed SpriteBatcher
	const auto arguments = lua_gettop(L) - 1;
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
	switch (arguments){
	case 2:
		sb->push(spt, v);
		break;
	case 3:
		sb->push(spt, v, luaP_tofloat(L, 4));
		break;
	case 4: 
		sb->push(spt, v, luaP_tofloat(L, 4), luaP_tofloat(L, 5));
		break;
	default:
		error.reportError("Sprite Batcher called with a invalid number of arguments");
	} 

	return 0;
}

int SpriteBatcher::lFlush(lua_State* L){
	static const auto sb = Util::luaP_touserdata<SpriteBatcher>(L, 1);
	sb->flush();
	return 0;
}