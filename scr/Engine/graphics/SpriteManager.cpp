

#include "SpriteManager.h"

using namespace Pro;
using namespace Graphics;
using namespace Asset;

SpriteManager::SpriteManager(lua_State* _lua_state)
{
	lua_state = _lua_state;
}


SpriteManager::~SpriteManager()
{
	// release all textures
}

game_id SpriteManager::loadSprite(const string& name,const CBuffer data){ 
	// Load in texture
	auto tex = IMG_LoadTexture(renderer, static_cast<const char*>(data.data)); 
	if(tex == nullptr)
		return 0;
	// Query texture dimensions
	auto dim = Vector2(); 
	SDL_QueryTexture(tex, NULL, NULL, (int*)&dim.x, (int*)&dim.y);

	// Store the texture as a sprite
	auto sprite = Sprite(name, dim, tex);
	auto spriteID = sprite.getGUID();
	sprites.insert({ spriteID, move(sprite) });
	return spriteID;
}

// opens a file with each line being a frame
Asset::AnimatedSprite SpriteManager::loadAnimation(const std::string& path){
	if (renderer == nullptr)
		renderer = luaP_getSDLRenderer(lua_state);
	Asset::AnimatedSprite anim;
	std::fstream stream(path);

	if (stream.eof())
		return anim;

	std::string line;
	line.resize(512);

	while (stream.eof() == false){
		stream.getline(&line[0], 512);
		// Check if the line is empty
		if (line[0] == '\0')
			continue;
		anim.addFrame(&sprites.at(GUIDLookup::getGUID(line)));
	}
	return anim;
}
 
Asset::Sprite* SpriteManager::getSprite(game_id id){
	return &sprites.at(id);
}

Asset::AnimatedSprite* SpriteManager::getAnim(game_id id){
	return animations.at(id);
}
 
void SpriteManager::release(game_id textureID){
	// will call sprited destructor and 
	// will delete the SDL_Texture
	sprites.erase(textureID);
}
 

int SpriteManager::lGetSprite(lua_State* L){
	const auto sm = Util::luaP_touserdata<SpriteManager>(L, 1);
	const auto s = sm->getSprite(static_cast<game_id>(lua_tonumber(L, 2)));
	Util::luaP_newobject<Sprite>(L, s);
	return 1;
}
int SpriteManager::lGetAnimation(lua_State* L){ 
	const auto sm = Util::luaP_touserdata<SpriteManager>(L, 1);
	const auto as = sm->getAnim(static_cast<game_id>(lua_tonumber(L, 2)));
	Util::luaP_newobject<AnimatedSprite>(L, as);
	return 1;
}