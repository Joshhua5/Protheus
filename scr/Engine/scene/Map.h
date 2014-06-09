/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	The world map is broken down into sections
	Each section is defined according to the datafile 
-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once
#include <SDL.h>
#include <string>
#include <fstream>
#include <vector>

#include "Camera.h"
#include "Tile.h"
#include "..\component\Area.h"
#include "..\util\StringConverter.h"
#include "MapSection.h"
#include "..\Math.h"

namespace Pro{
	namespace Scene{
		class Map : 
			public Component::Area
		{
		private:  
			std::vector<MapSection*> mapSections;
			std::vector<TileType> tileData; 

			bool loadLevelData(const std::string& file);
			bool loadLevelTileData(const std::string&file);
		public:
			Map();
			~Map();

			std::vector<MapSection*> getVisibleSections(Camera* cam);
			bool loadLevel(const std::string& file, const std::string& data);
			TileType* getTile(Math::Vector2&);
			TileType* getTileType(unsigned int);

			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "scene_map_metatable";
			}
		};
	} 
}