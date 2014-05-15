#include "Renderer.h"

namespace Pro{
	namespace Graphics{

		Renderer::Renderer(SpriteManager* _spriteMng, Scene* _scene)
		{ 
			spriteMng = _spriteMng;
			scene = _scene;
		}


		Renderer::~Renderer()
		{
			delete sprite_batcher;
		}

		bool Renderer::init(SDL_Window* window){
			renderer = SDL_CreateRenderer(window, -1,
				SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == nullptr)
				return false;
			sprite_batcher = new SpriteBatcher(renderer);
			return true;
		}

		SDL_Renderer* Renderer::getRenderer(){
			return renderer;
		}

		void Renderer::renderScene(){
			// Get data
			Map* map = scene->getMap();
			//SDL_Point* cameraPos = scene->getActiveCamera()->getPosition();

			std::unordered_map<ID, Entity*>* entities = scene->getEntities();

			// Render the Map, only sections visible to the camera

			auto sections = map->getVisibleSections(scene->getActiveCamera());

			// render the tiles

			int x(0), y(0);

			for each(const auto& section in sections){
				for each(const auto& col in section->getData()){
					if (col[0] == '\r')
						continue;
					for each(const auto& row in col){
						TileType* tile = map->getTileType(row - 48);
						// check if textured
						if (!tile->isTextured)
							continue;
						// populate the sprite pointer
						if (tile->sprite == nullptr)
							tile->sprite = spriteMng->getSprite(tile->spriteName);

						SDL_Rect spriteRect = tile->sprite->getRect();
						spriteRect.x *= spriteRect.w;
						spriteRect.y *= spriteRect.h;
						sprite_batcher->push(tile->sprite, spriteRect); 
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
				aeCache = dynamic_cast<AnimatedEntity*>(kv.second);
				if (aeCache != nullptr){ 
					sprite_batcher->push(aeCache->activeAnimation->getFrame(), aeCache->getPositionRect());
					aeCache->activeAnimation->nextFrame();
					continue;
				}
				// check if has sprite
				seCache = dynamic_cast<SpriteEntity*>(kv.second);
				if (seCache != nullptr){
					sprite_batcher->push(spriteMng->getSprite(seCache->getSpriteName()), seCache->getPositionRect());
					continue;
				}
				continue;
			}
			// calls the batcher to render all sprites
			sprite_batcher->flush();
		}

		
	}
}