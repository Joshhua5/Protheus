/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Creates all the global's which are accessible through Lua.
-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include "lib\lua.hpp" 
#include "..\GameObjects.h"
#include "..\Math.h"
#include "..\graphics\Renderer.h"
#include "..\graphics\Sprite.h"
#include "..\Components.h"
#include "..\gui\GUIContext.h"
#include "..\gameobject\Scene.h"

#include <vector>

using namespace std;


namespace Pro{
	using namespace Component;
	using namespace GameObject; 
	using namespace Graphics;
	using namespace GUI;
	using namespace Math;
	using namespace Asset;

	namespace Lua{
		class LuaMetatableFactory{
			typedef std::vector<luaL_Reg> Metatable;
			   
			template<typename T> inline void saveMetatable(lua_State* L, Metatable& fields){
				luaL_newmetatable(L, &T::lGetMetatable()[0]);

				for each(auto field in fields){
					lua_pushcfunction(L, field.func);
					lua_setfield(L, -2, field.name);
				}

				lua_pushstring(L, "__index");
				lua_pushvalue(L, -2);
				lua_settable(L, -3);
			}
			
			template<typename T> inline void defineMetatable(lua_State* L){
				Metatable fields;
				T::lGetFunctions<T>(fields);
				saveMetatable<T>(L, fields);
			}

		public:
			LuaMetatableFactory(lua_State* L){

				// Components

				defineMetatable<ActiveState>(L);
				defineMetatable<Area>(L);
				defineMetatable<CGUID>(L);
				defineMetatable<CScriptable>(L);
				defineMetatable<LuaCallback>(L);
				defineMetatable<Name>(L);
				defineMetatable<Position>(L);

				// Game Objects

				defineMetatable<Avatar>(L);
				defineMetatable<Camera>(L);
				defineMetatable<Scene>(L);
				defineMetatable<AnimatedEntity>(L); 
				defineMetatable<Entity>(L);
				defineMetatable<Inventory>(L);
				defineMetatable<Item>(L);
				defineMetatable<Map>(L);
				defineMetatable<MapSection>(L);
				defineMetatable<SpriteEntity>(L);
				defineMetatable<TileType>(L);

				// GUI

				//defineMetatable<GUIButton>(L);
				//defineMetatable<GUIContext>(L);
				//defineMetatable<GUIContext>(L);
				//defineMetatable<GUIWindow>(L);

				// Audio

				// Event

				// IO

				// Graphics

				defineMetatable<Sprite>(L); 
				defineMetatable<AnimatedSprite>(L);
				defineMetatable<SpriteManager>(L);
				defineMetatable<SpriteBatcher>(L);
				defineMetatable<Renderer>(L);

				// Containers
				 
				// Math

				defineMetatable<Vector2>(L);
				defineMetatable<Vector3>(L);
				defineMetatable<Vector4>(L);

				// Scene

				// Networking


				//  
			}
			LuaMetatableFactory(); 
		};
	}
}