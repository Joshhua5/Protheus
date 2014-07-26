#include "Renderer.h"

using namespace Pro;
using namespace Graphics;
using namespace Asset;
  
Renderer::Renderer(lua_State* _lua_state){
    lua_state = _lua_state;
	SDL_Window* w = luaP_getSDLWindow(lua_state);
	
	renderer = SDL_CreateRenderer(w, -1,
		SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == nullptr){
		error.reportFatal("Failure to create Renderer");
		return;
	} 

	luaP_setSDLRenderer(lua_state, renderer); 
	sprite_batcher = new SpriteBatcher(lua_state);
}

Renderer::~Renderer()
{
	delete sprite_batcher;
}

SDL_Renderer* Renderer::getRenderer(){
	return renderer;
}

void Renderer::renderScene(Scene* scene, SpriteManager* spriteMng){
	// Get data
	Map* map = scene->getMap();
	//SDL_Point* cameraPos = scene->getActiveCamera()->getPosition();

	//std::unordered_map<GUID, GameObject::Entity*>* entities = scene->getEntities();

	// Render the Map, only sections visible to the camera

	auto sections = map->getVisibleSections(*scene->getActiveCamera());

	// render the tiles

	int x(0), y(0);

	for each(const auto section in sections){
		for each(const auto col in section->getData()){
			if (col[0] == '\r')
				continue;
			for each(const auto& row in col){
				TileType* tile = map->getTileType(row - 48);
				// check if textured
				if (!tile->isTextured)
					continue;
				// populate the sprite pointer
				if (static_cast<Sprite*>(tile->spriteCache) == nullptr)
					tile->spriteCache = spriteMng->getSprite(GUIDLookup::getGUID(tile->spriteName));

				auto spriteRect = static_cast<Asset::Sprite*>(tile->spriteCache)->getDimensions();
				auto position = Vector2(spriteRect.x * x, spriteRect.y * y);
				sprite_batcher->push(static_cast<Sprite*>(tile->spriteCache), Vector4(position, spriteRect));
				y++;
			}
			++x;
			y = 0;
		}
		x = 0;
	}

	// render entities
	//GameObject::AnimatedEntity* aeCache;
	//GameObject::SpriteEntity* seCache;
	//for each(auto kv in *entities){
	//	// check if animated
	//	aeCache = dynamic_cast<GameObject::AnimatedEntity*>(kv.second);
	//	if (aeCache != nullptr){
	//		sprite_batcher->push(aeCache->activeAnimation->getFrame(), aeCache->getPositionRect());
	//		aeCache->activeAnimation->nextFrame();
	//		continue;
	//	}
	//	// check if has sprite
	//	seCache = dynamic_cast<GameObject::SpriteEntity*>(kv.second);
	//	if (seCache != nullptr){
	//		sprite_batcher->push(spriteMng->getSprite(seCache->getSpriteName()), seCache->getPositionRect());
	//		continue;
	//	}
	//	continue;
	//}
	// calls the batcher to render all sprites

	sprite_batcher->flush();
}

void Renderer::startFrame(){
	SDL_RenderClear(renderer);
}

void Renderer::endFrame(){ 
	SDL_RenderPresent(renderer);
}

SpriteBatcher* Renderer::getBatcher(){
	return sprite_batcher;
}

int Renderer::lGetBatcher(lua_State* L){
	const auto r = Util::luaP_touserdata<Renderer>(L, -1);
	Util::luaP_newobject<SpriteBatcher>(L, r->getBatcher());
	return 1;
}

int Renderer::lRenderScene(lua_State* L){
	Renderer* r = Util::luaP_touserdata<Renderer>(L, 1);
	r->renderScene(
		Util::luaP_touserdata<Scene>(L, 2),
		Util::luaP_touserdata<SpriteManager>(L, 3)
		);
	return 0;
}