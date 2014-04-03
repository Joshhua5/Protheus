#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Avatar.h"
#include "Camera.h"
#include "IDManager.h"
#include "TileMap.h"
#include "Tile.h"

namespace Pro{

	class Scene
	{
		TileMap* tileData;
		Camera* activeCamera;
		std::unordered_map<ID, Entity> entities;
		std::unordered_map<ID, Camera*> cameras;
	public:
		Scene();
		~Scene();

		bool loadSceneData(const std::string& path, const std::string& data);
		bool addTileType(TileType);

		// returns a list of entities found on a tile
		std::vector<Entity*> pollTile(unsigned int x, unsigned int y);
		// returns a list of entities found on a tile
		std::vector<Entity*> pollTile(SDL_Rect pos);

		// get's an ID
		Entity* getEntity(ID); 
		// allows the id to be retrieved via the name
		void addEntity(Entity, ID);
		// id must be managed, if lost the entity can't be retrieved. 

		bool destoryEntity(ID);
		bool destoryEntity(Entity*); 

		Camera* getCamera(ID);
		Camera* getActiveCamera();
		void setActiveCamera(ID);
		void addCamera(ID, Camera);

		// return the private storage of entities
		std::unordered_map<ID, Entity>* getEntities();
		TileMap* getMapData();
	};
}