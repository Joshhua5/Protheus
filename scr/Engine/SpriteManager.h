#pragma once
#include <string>
#include <vector> 
#include <unordered_map>
#include <thread>
#include <fstream>
#include <SDL_image.h>
#include "AnimatedSprite.h" 
namespace Pro{
	namespace Graphics{
	class SpriteManager
	{
		SDL_Texture* spriteSheet;
		std::unordered_map<std::string, Sprite> sprites;
		std::unordered_map<std::string, AnimatedSprite> animations;

		AnimatedSprite loadAnimation(SDL_Renderer *renderer, const std::string& path);
	public:
		SpriteManager();
		~SpriteManager();

		Sprite* getSprite(const std::string &_name);
		AnimatedSprite* getAnim(const std::string& name);
		SDL_Texture* getSpriteSheet();

		bool loadSpriteSheet(SDL_Renderer *renderer, const std::string& image, const std::string& data);
		bool loadAnimations(SDL_Renderer* renderer, const std::string& path);
		void release();
	};
	}
}