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

		public:
			Map();
			~Map();
			 
			vector<MapSection*> getSections(){ return mapSections; }
			unsigned int getSectionCount();
			vector<MapSection*> getVisibleSections(Camera* cam);
			// function used in loading data
			// Accepts a MapSection and moves it in
			void addSection(MapSection&);

			// returns the tile which is at a position
			TileType* getTile(Math::Vector2&); 
			TileType* getTileType(unsigned int);
			// returns the whole TileData vector
			vector<TileType> getTileData();
			// Accepts a reference to tile data
			// and moves it into the Map
			void setTileData(vector<TileType>& data);
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