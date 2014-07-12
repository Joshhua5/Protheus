#include "SpriteBatcher.h"

using namespace Pro;
using namespace Graphics;
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
	render_stack.push({ _s, _r });
}

void SpriteBatcher::flush(){
	for (auto x = render_stack.size(); x != 0; --x){
		const auto pair = &render_stack.top();
		auto dstRect = pair->second.toSDL();
		SDL_RenderCopy(renderer, pair->first->getTexture(), NULL, &dstRect);
		render_stack.pop();
	}
}

int SpriteBatcher::lPush(lua_State* L){
	const auto sb = Util::luaP_touserdata<SpriteBatcher>(L, 1);
	const auto spt = Util::luaP_touserdata<Asset::Sprite>(L, 2);
	auto v = Math::Vector4(*Util::luaP_touserdata<Math::Vector2>(L, 3), spt->getDimensions());
	sb->push(spt, v); 
	return 0;
}

int SpriteBatcher::lFlush(lua_State* L){
	const auto sb = Util::luaP_touserdata<SpriteBatcher>(L, 1);
	sb->flush();
	return 0;
}