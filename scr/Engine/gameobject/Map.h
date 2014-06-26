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

#include "..\util\StringConverter.h"
#include "MapSection.h"

using namespace std;

namespace Pro{
	namespace GameObject{
		class Map : 
			public Component::Area
		{
		private:  
			vector<MapSection*> mapSections;
			vector<TileType> tileData;

			bool loadLevelData(const std::string& file);
			bool loadLevelTileData(const std::string&file);
		public:
			Map();
			~Map();

			// returns a single map section which contains 
			// all map data
			MapSection* condenseMap();
			vector<MapSection*> getSections(){ return mapSections; }
			vector<MapSection*> getVisibleSections(Camera* cam);

			bool loadLevel(const std::string& file, const std::string& data);

			// returns the tile which is at a position
			TileType* getTile(Math::Vector2&); 
			TileType* getTileType(unsigned int);
			// returns the whole TileData vector
			vector<TileType> getTileData();
			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "scene_map_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				Area::lGetFunctions<T>(fields);
			}
		};
	} 
}