/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <vector>
#include "Tile.h"
#include "DataEntity.h"
#include "..\Math.h"
#include "..\Components.h"
#include <SDL.h>

namespace Pro{
	namespace GameObject{
		using namespace std;
		using namespace Component;
		using namespace Math;

		class MapSection :
			public Area,
			public Position
		{
			vector<vector<short>> data;
		public:
			short tileAt(Vector2&);
			// returns a pointer so we can return nullptr if
			// a tile isn't found
			short* contains(Vector2&);
			void setData(vector<vector<short>>);
			//void setData(short*);
			vector<vector<short>> getData();
			bool visible(DataEntity& cam);

			constexpr static const char* lGetMetatable(){
				return "map_section_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(vector<luaL_Reg>& fields){
				Area::lGetFunctions<T>(fields);
				Position::lGetFunctions<T>(fields);
			}
		};
	}
}