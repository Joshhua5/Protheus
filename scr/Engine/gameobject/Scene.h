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

#include "..\gameobject\Avatar.h"
#include "..\containers\EntityContainer.h"
#include "..\containers\CameraContainer.h"
#include "..\component\CGUID.h"
#include "..\util\LuaUtils.h"
#include "..\Math.h"

#include "Tile.h"
#include "Map.h"
#include "Camera.h"
namespace Pro{
	namespace GameObject{
		class Scene :
			public EntityContainer,
			public CameraContainer,
			public Component::CGUID
		{
			Map* tileData; 
		public:
			lua_State* lua_state;
			Scene(lua_State* lua_state);
			Scene(){};
			~Scene();

			bool addTileType(TileType);
			void update();

			// returns a list of entities found on a tile
			std::vector<GameObject::Entity*> pollTile(Math::Vector2&); 
			 
			// return the private storage of entities 
			Map* getMap();

			// LUA functions

			static int lUpdate(lua_State*);

			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "scene_metatable";
			}

			template<typename T> 
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				CGUID::lGetFunctions<T>(fields);
				fields.push_back({ "update", (lua_CFunction)&T::lUpdate });
			}
		};
	}
}