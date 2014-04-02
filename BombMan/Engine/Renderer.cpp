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
		TileMap* map = scene->getMapData();
		SDL_Rect cameraPos = scene->getActiveCamera()->getPosition();
		std::vector<TileType>* tiles = &map->tileData;
		std::unordered_map<ID, Entity>* entities = scene->getEntities();

		// Render the Map, only tiles visible to the camera

		if ((cameraPos.x + cameraPos.w) - map->width > 0)
			// camera overhangs map
			cameraPos.w -= (cameraPos.x + cameraPos.w) - map->width;
		if ((cameraPos.y + cameraPos.h) - map->height > 0)
			// camera overhangs map
			cameraPos.h -= (cameraPos.y + cameraPos.h) - map->height ;

		// render the tiles

		// check if world is smaller than camera space

		for (int x = cameraPos.x; x < cameraPos.x + cameraPos.w - 1; x++){
			for (int y = cameraPos.y; y < cameraPos.y + cameraPos.h; y++){
				
				// Map reads down and to the right
				
				// Convert Ascii to integer
				TileType* tile = &tiles->at(map->data[y][x] - 48); 
				// check if textured
				if (!tile->isTextured)
					continue;
				// populate the sprite pointer
				if (tile->sprite == nullptr)
					tile->sprite = spriteMng->getSprite(tile->spriteName);
				
			/*	SDL_Rect sizeCache = tiles->at(map->data[y][x]).tileSize;

				sizeCache.x = sizeCache.w * x;
				sizeCache.y = sizeCache.h * y;*/
				renderSprite(tile->sprite, tile->sprite->getRect());

			}
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