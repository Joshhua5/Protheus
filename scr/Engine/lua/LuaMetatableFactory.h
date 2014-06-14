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
#include "..\Components.h"
#include "..\gui\GUIContext.h"
#include "..\gameobject\Scene.h"

#include <vector>

using namespace std;


namespace Pro{
	using namespace Component;
	using namespace GameObject; 
	using namespace GUI; 

	namespace Lua{
		class LuaMetatableFactory{
			typedef std::vector<luaL_Reg> Metatable;
			   
			template<typename T> void saveMetatable(lua_State* L, Metatable& fields){
				luaL_newmetatable(L, &T::lGetMetatable()[0]);

				for each(auto field in fields){
					lua_pushcfunction(L, field.func);
					lua_setfield(L, -2, field.name);
				}

				lua_pushstring(L, "__index");
				lua_pushvalue(L, -2);
				lua_settable(L, -3);
			}
			
			template<typename T> void defineMetatable(lua_State* L){
				Metatable fields;
				T::lGetFunctions<T>(fields);
				saveMetatable<T>(L, fields);
			}

			template<> void defineMetatable<ActiveState>(lua_State* L){
				Metatable fields;
				ActiveState::lGetFunctions<ActiveState>(fields);
				saveMetatable<ActiveState>(L, fields);
			}

			template<> void defineMetatable<Area>(lua_State* L){
				Metatable fields;
				Area::lGetFunctions<Area>(fields);
				saveMetatable<Area>(L, fields);
			}

			template<> void defineMetatable<CGUID>(lua_State* L){
				Metatable fields;
				CGUID::lGetFunctions<CGUID>(fields);
				saveMetatable<CGUID>(L, fields);
			}

			template<> void defineMetatable<CScriptable>(lua_State* L){
				Metatable fields;
				CScriptable::lGetFunctions<CScriptable>(fields);
				saveMetatable<CScriptable>(L, fields);
			}

			template<> void defineMetatable<LuaCallback>(lua_State* L){
				Metatable fields;
				LuaCallback::lGetFunctions<LuaCallback>(fields);
				saveMetatable<LuaCallback>(L, fields);
			}

			template<> void defineMetatable<Name>(lua_State* L){
				Metatable fields;
				Name::lGetFunctions<Name>(fields);
				saveMetatable<Name>(L, fields);
			}

			template<> void defineMetatable<Position>(lua_State* L){
				Metatable fields;
				Position::lGetFunctions<Position>(fields);
				saveMetatable<Position>(L, fields);
			}

			template<> void defineMetatable<Avatar>(lua_State* L){
				Metatable fields; 
				Avatar::lGetFunctions<Avatar>(fields);
				CGUID::lGetFunctions<Avatar>(fields);
				Position::lGetFunctions<Avatar>(fields);
				Inventory::lGetFunctions<Avatar>(fields);
				AnimatedEntity::lGetFunctions<Avatar>(fields); 
				saveMetatable<Avatar>(L, fields);
			}

			template<> void defineMetatable<Camera>(lua_State* L){
				Metatable fields;
				Camera::lGetFunctions<Camera>(fields);
				CGUID::lGetFunctions<Camera>(fields);
				Position::lGetFunctions<Camera>(fields);
				Area::lGetFunctions<Camera>(fields);
				saveMetatable<Camera>(L, fields);
			}

			template<> void defineMetatable<Scene>(lua_State* L){
				Metatable fields;
				Scene::lGetFunctions<Scene>(fields);
				CGUID::lGetFunctions<Scene>(fields); 

				saveMetatable<Scene>(L, fields);
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
				/*defineMetatable<AnimatedEntity>(L);
				defineMetatable<Entity>(L);
				defineMetatable<Inventory>(L);
				defineMetatable<Item>(L);
				defineMetatable<Map>(L);
				defineMetatable<MapSection>(L);
				defineMetatable<SpriteEntity>(L);
				defineMetatable<TileType>(L);*/

				// GUI

				//defineMetatable<GUIButton>(L);
				//defineMetatable<GUIContext>(L);
				//defineMetatable<GUIContext>(L);
				//defineMetatable<GUIWindow>(L);

				// Audio

				// Event

				// IO

				// Graphics

				// Containers
				 
				// Math

				// Scene

				// Networking


				//  
			}
			LuaMetatableFactory(); 
		};
	}
}