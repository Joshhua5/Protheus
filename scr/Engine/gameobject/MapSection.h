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
#include "Camera.h"
#include "..\Math.h"
#include "..\component\Area.h"
#include <SDL.h>

namespace Pro{
	namespace GameObject{

		class MapSection : 
			public Component::Area,
			public Component::Position
		{ 
			std::vector<std::vector<char>> data;
		public:
			char tileAt(Math::Vector2&);
			char* contains(Math::Vector2&); 
			void setData(std::vector<std::vector<char>>);
			std::vector<std::vector<char>> getData();
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