/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description: 

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.
*************************************************************************/


#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Avatar.h"
#include "Camera.h"
#include "GUIDLookup.h"
#include "CameraContainer.h"
#include "EntityContainer.h"
#include "Math.h"
#include "Map.h"
#include "Tile.h"

namespace Pro{
	namespace Scene{
		class Scene :
			public EntityContainer,
			public CameraContainer
		{
			Map* tileData; 
		public:
			Scene();
			~Scene();

			bool loadSceneData(const std::string& path, const std::string& data);
			bool addTileType(TileType);

			// returns a list of entities found on a tile
			std::vector<GameObject::Entity*> pollTile(Math::Vector2); 
			 
			// return the private storage of entities 
			Map* getMap();
		};
	}
}