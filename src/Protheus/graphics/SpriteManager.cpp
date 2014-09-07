#include "SpriteManager.h"

using namespace Pro;
using namespace Graphics;
using namespace Asset;

SpriteManager::SpriteManager(lua_State* _lua_state)
{
	lua_state = _lua_state;
	renderer = luaP_getSDLRenderer(_lua_state);
}

SpriteManager::~SpriteManager()
{
	// release all textures
	for each(auto& spt in sprites)
		delete spt.second;
}

Sprite* SpriteManager::loadSprite(const string& name, const string& imagePath){
	// Load in texture 
	auto tex = IMG_LoadTexture(renderer, imagePath.data());
	if (tex == nullptr){ 
		error.reportError(
			"Unable to load image: " + name + "\n" +
			"Image Path :" + imagePath + "\n" +
			IMG_GetError()); 
		return nullptr;
	} 
	// Query texture dimensions
	auto dim = Vector2();
	int dimx = 0;
	int dimy = 0;
	SDL_QueryTexture(tex, NULL, NULL, &dimx, &dimy);

	dim.x = static_cast<float>(dimx);
	dim.y = static_cast<float>(dimy);

	// Store the texture as a sprite
	const auto sprite = new Sprite(name, dim, tex);
	const auto spriteID = sprite->getGUID();
	sprites.insert({ spriteID, sprite });
	return sprite;
}

// opens a file with each line being a frame
AnimatedSprite SpriteManager::loadAnimation(const std::string& path){
	if (renderer == nullptr)
		renderer = luaP_getSDLRenderer(lua_state);
	AnimatedSprite anim;
	fstream stream(path);

	if (stream.eof())
		return anim;

	string line;
	line.resize(512);

	while (stream.eof() == false){
		stream.getline(&line[0], 512);
		// Check if the line is empty
		if (line[0] == '\0')
			continue;
		anim.addFrame(sprites.at(GUIDLookup::getGUID(line)));
	}
	return anim;
}

Asset::Sprite* SpriteManager::getSprite(const game_id id){
	return sprites.at(id);
}

Asset::AnimatedSprite* SpriteManager::getAnim(const game_id id){
	return animations.at(id);
}

void SpriteManager::release(const game_id textureID){
	sprites.at(textureID)->releaseTexture();
	sprites.erase(textureID);
}

int SpriteManager::lLoadSprite(lua_State* L){
	const auto sm = Util::luaP_touserdata<SpriteManager>(L, 1);
	const auto fs = luaP_getFileSystem(L);  
	Util::luaP_newobject(L, sm->loadSprite(lua_tostring(L, 2), fs->getRootDir() + lua_tostring(L, 3))); 
	return 1;
}

int SpriteManager::lGetSprite(lua_State* L){
	const auto sm = Util::luaP_touserdata<SpriteManager>(L, 1);
	const game_id id = luaP_togameid(L, 2);

#ifdef DEBUG

	if (sm == nullptr){
		error.reportError("SpriteManager called without :");
		return 0;
	}

	if (id == 0){
		error.reportError("Null Game ID passed, function ignored");
		return 0; 
	} 

#endif

	const auto s = sm->getSprite(id);
	Util::luaP_newobject<Sprite>(L, s);
	return 1;
}
int SpriteManager::lGetAnimation(lua_State* L){
	const auto sm = Util::luaP_touserdata<SpriteManager>(L, 1);

#ifdef DEBUG

	if (sm == nullptr){
		error.reportError("SpriteManager called without :");
		return 0;
	}

	if (lua_tonumber(L, 2) == 0){
		error.reportError("Null Game ID passed, function ignored");
		return 0;
	}

#endif
	const auto as = sm->getAnim(luaP_togameid(L, 2)); 
	Util::luaP_newobject<AnimatedSprite>(L, as);
	return 1;
}