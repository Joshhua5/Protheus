#include "SpriteManager.h"
namespace Pro{


	SpriteManager::SpriteManager()
	{
	}


	SpriteManager::~SpriteManager()
	{
	}

	// opens a file with each line being a frame
	AnimatedSprite SpriteManager::loadAnimation(SDL_Renderer *renderer, const std::string& path){
		AnimatedSprite anim;
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
			  
			AnimatedSprite anim = loadAnimation(renderer,
				line.substr(line.find_first_of(':') + 1, line.find(';') - line.find(':') - 1));

			animations.insert({ key, anim });
		}
		stream.close();
		return true;
	}

	bool SpriteManager::loadSpriteSheet(SDL_Renderer *renderer, const std::string &path, const std::string& data){
		std::vector<std::string> lines;
		std::fstream stream(data);

		if (!stream.is_open() || stream.bad()){
			return false;
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
			  
			unsigned int pos = lines[x].find('=') + 1;

			int opp = lines[x].find_first_of(' ', pos + 1) - lines[x].find_first_of(' ') - 1;



			rect.x = atoi(&lines[x].substr(pos, opp)[0]);
			// add length onto position
			pos += opp;
			// get the next length for .y
			opp = lines[x].find_first_of(' ', pos + 1) - lines[x].find_first_of(' ') - 1;
			// get the .y
			rect.y = atoi(&lines[x].substr(pos, opp)[0]);
			pos += opp;
			opp = lines[x].find_first_of(' ', pos + 1) - lines[x].find_first_of(' ') - 1;

			rect.w = atoi(&lines[x].substr(pos, opp)[0]);	
			pos += opp;
			opp = lines[x].find_first_of(' ', pos + 1) - lines[x].find_first_of(' ') - 1;

			rect.h = atoi(&lines[x].substr(pos, opp)[0]);	
			pos += opp;
			opp = lines[x].find_first_of(' ', pos + 1) - lines[x].find_first_of(' ') - 1;

			
			std::string name = (lines[x].substr(0, lines[x].find_first_of(' ', 0))).c_str();

			Sprite sprite(name, rect);
			sprite.attachSpriteSheet(spriteSheet); 
			sprites.insert({ name, sprite });
		}
		delete [] buffer;
		 
		return true;
	}

	Sprite* SpriteManager::getSprite(const std::string& name){
		return &sprites.at(name);
	}

	AnimatedSprite* SpriteManager::getAnim(const std::string& name){
		return &animations.at(name);
	}

	SDL_Texture* SpriteManager::getSpriteSheet(){
		return spriteSheet;
	}

	void SpriteManager::release(){
		SDL_DestroyTexture(spriteSheet);
	}
}