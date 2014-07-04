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
#include "..\component\Position.h" 
#include "..\Math.h"
#include "..\component\Area.h"
#include <SDL.h>

namespace Pro{
	namespace GameObject{

		class MapSection : 
			public Component::Area,
			public Component::Position
		{ 
			std::vector<std::vector<short>> data;
		public:
			short tileAt(Math::Vector2&);
			// returns a pointer so we can return nullptr if
			// a tile isn't found
			short* contains(Math::Vector2&); 
			void setData(std::vector<std::vector<short>>);
			//void setData(short*); 
			std::vector<std::vector<short>> getData();
			bool visible(Camera* cam);

			static string lGetMetatable(){
				return "map_section_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				Area::lGetFunctions<T>(fields);
				Position::lGetFunctions<T>(fields); 
			}
		};
	}
}