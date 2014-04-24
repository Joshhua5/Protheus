#pragma once
#include "Sprite.h"
#include "Scene.h"
#include <unordered_map>
#include "SpriteManager.h"
#include <SDL.h>
namespace Pro{
	namespace Graphics{
	class Renderer
	{
	private:
		SDL_Renderer* renderer;
		SpriteManager* spriteMng;
		Scene* scene;
	public:
		Renderer(SpriteManager*, Scene*);
		~Renderer();

		bool init(SDL_Window* window);
		SDL_Renderer* getRenderer();

		bool renderSprite(Sprite*, SDL_Rect position);

		void renderScene( );
	}; 
	}
}