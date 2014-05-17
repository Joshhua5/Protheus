#include "SpriteManager.h"
namespace Pro{
	namespace Graphics{

		SpriteManager::SpriteManager()
		{
		}


		SpriteManager::~SpriteManager()
		{
		}

		// opens a file with each line being a frame
		Asset::AnimatedSprite SpriteManager::loadAnimation(SDL_Renderer *renderer, const std::string& path){
			Asset::AnimatedSprite anim;
			std::fstream stream(path);

			if (stream.eof())
				return anim;

			std::string line;
			line.resize(512);

			while (stream.eof() == false){
				stream.getline(&line[0], 512);
				// common sense check
				if (line[0] == '\0')
					continue;
				anim.addFrame(&sprites.at(line));
			}
			return anim;
		}

		// a directory of animations
		bool SpriteManager::loadAnimations(SDL_Renderer *renderer, const std::string& path){
			std::fstream stream(path);

			if (!stream.is_open() || stream.bad())
				return false;

			if (stream.eof() == true)
				return false;

			std::string line;
			line.resize(512);

			while (stream.eof() == false){
				// File structure
				// Key:Path;
				stream.getline(&line[0], 512);
				std::string key = line.substr(0, line.find_first_of(':'));

				Asset::AnimatedSprite anim = loadAnimation(renderer,
					line.substr(line.find_first_of(':') + 1, line.find(';') - line.find(':') - 1));

				animations.insert({ key, anim });
			}
			stream.close();
			return 0;
		}

		bool SpriteManager::loadSpriteSheet(SDL_Renderer *renderer, const std::string &path, const std::string& data){
			std::vector<std::string> lines;
			std::fstream stream(data);

			if (!stream.is_open() || stream.bad()){
				return 1;
			}

			char* buffer = new char[512];

			while (!stream.eof()){
				stream.getline(buffer, 512);
				if (buffer[0] != '\0')
					lines.push_back(buffer);
			}

			// load the SpriteSheet
			spriteSheet = IMG_LoadTexture(renderer, &path[0]);

			// decode the data 

			for (unsigned int x = 0; x < lines.size(); x++){
				// Check if the line at least matchs the minimum characters
				// required to form a correct definition (to prevent empty lines)
				if (lines[x].length() < 10)
					continue;
				SDL_Rect rect;

				//unsigned int pos = lines[x].find('=') + 1;

				//int opp = lines[x].find_first_of(' ', pos + 1) - lines[x].find_first_of(' ') - 1;



				std::vector<std::string> elements;
				// break string down into elements
				//name = x y w h 
				int pos = 0;
				int length;
				// Get name
				elements.push_back(lines[x].substr(0, lines[x].find_first_of(' ', 0)));
				// find the first ' '
				pos = lines[x].find(' ', pos);
				for (int i = 0; i < 6; i++){
					// grab the length between ' '
					length = lines[x].find(' ', pos + 1) - pos;
					elements.push_back(
						lines[x].substr(pos + 1, length));
					// move the position for the next value
					pos += length;
				}
				// convert the elements into the integer
				rect.x = atoi(elements.at(2).c_str());
				rect.y = atoi(elements.at(3).c_str());
				rect.w = atoi(elements.at(4).c_str());
				rect.h = atoi(elements.at(5).c_str());

				Asset::Sprite sprite(elements.at(0), rect);
				sprite.attachSpriteSheet(spriteSheet);
				sprites.insert({ elements.at(0), sprite });
			}
			delete[] buffer;

			return 0;
		}

		Asset::Sprite* SpriteManager::getSprite(const std::string& name){
			return &sprites.at(name);
		}

		Asset::AnimatedSprite* SpriteManager::getAnim(const std::string& name){
			return &animations.at(name);
		}

		SDL_Texture* SpriteManager::getSpriteSheet(){
			return spriteSheet;
		}

		void SpriteManager::release(){
			SDL_DestroyTexture(spriteSheet);
		}
	}
}