#include "Renderer.h"

namespace Pro{

	Renderer::Renderer(SpriteManager* _spriteMng, Scene* _scene)
	{
		spriteMng = _spriteMng;
		scene = _scene;
	}


	Renderer::~Renderer()
	{
	}

	bool Renderer::init(SDL_Window* window){
		renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == nullptr)
			return false;
		return true;
	}

	SDL_Renderer* Renderer::getRenderer(){
		return renderer;
	}

	void Renderer::renderScene(){
		// Get data
		Map* map = scene->getMap();
		SDL_Rect cameraPos = scene->getActiveCamera()->getPosition(); 
		std::unordered_map<ID, Entity>* entities = scene->getEntities();

		// Render the Map, only tiles visible to the camera

		auto sections = map->getVisibleSections(scene->getActiveCamera());

		// render the tiles

		// check if world is smaller than camera space
		int x(0), y(0);

		for each(const auto& section in sections){
			for each(const auto& col in section->getData()){ 
				for each(const auto& row in col){
					TileType* tile = map->getTileType(row - 48);
					// check if textured
					if (!tile->isTextured)
						continue;
					// populate the sprite pointer
					if (tile->sprite == nullptr)
						tile->sprite = spriteMng->getSprite(tile->spriteName);

					SDL_Rect spriteRect = tile->sprite->getRect();
					SDL_Rect position;
					position.x = x * spriteRect.w;
					position.y = y * spriteRect.h;
					position.w = spriteRect.w;
					position.h = spriteRect.h;
					renderSprite(tile->sprite, position);
					y++;
				}
				x++;
				y = 0;
			}
			x = 0;
		}

		// render entities 
		AnimatedEntity* aeCache;
		SpriteEntity* seCache;
		for each(auto kv in *entities){
			// check if animated
			aeCache = dynamic_cast<AnimatedEntity*>(&kv.second);
			if (aeCache != nullptr){
				renderSprite(aeCache->activeAnimation->getFrame(), aeCache->getPositionRect());
				aeCache->activeAnimation->nextFrame();
				continue;
			}
			// check if has sprite
			seCache = dynamic_cast<SpriteEntity*>(&kv.second);
			if (seCache != nullptr){
				renderSprite(spriteMng->getSprite(seCache->getSpriteName()), seCache->getPositionRect());
				continue;
			}
			continue;
		}

	}
	 
	bool Renderer::renderSprite(Sprite* spt, SDL_Rect position){
		if (SDL_RenderCopy(renderer, spt->getSpriteSheet(), &spt->getRect(), &position) == 0)
			return true;
		return false;
	} 
} 