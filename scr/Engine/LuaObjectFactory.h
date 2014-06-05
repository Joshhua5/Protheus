/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	assigns function pointers to Lua globals, so they can be called in
	Lua, also defines the Object creation for Lua so C++ objects can be
	created via Lua.

-------------------------------------------------------------------------
History:
- 21:05:2014 Waring J.
*************************************************************************/
#pragma once
#include "lua\lua.hpp" 
#include "Avatar.h"
#include "SceneContainer.h"
#include "LuaUtils.h"
#include "GraphicalUI.h"

using namespace std;

namespace Pro{
	namespace Lua{
		enum struct CLASS_FLAG{
			ACTIVE_STATE =			0x1,
			AREA =					0x2,
			GUID =					0x4,
			SCRIPTABLE =			0x8,
			LUA_CALLBACK =			0x10,
			NAME =					0x20,
			POSITION =				0x40,
			ENTITY =				0x80,
			INVENTORY =				0x100,
			ITEM =					0x200,
			SPRITE_ENTITY =			0x400,
			ANIMATED_ENTITY =		0x800,
			AVATAR =				0x1000,
			ANIMATED_SPRITE =		0x2000,
			SPRITE =				0x4000,
			CAMERA =				0x8000,
			GUI_ENTITY =			0x10000,
			GUI_BUTTON =			0x20000,

		};
		inline CLASS_FLAG operator|(CLASS_FLAG a, CLASS_FLAG b){
			return static_cast<CLASS_FLAG>(static_cast<int>(a) | static_cast<int>(b));
		}


		class CLua;

		class LuaObjectFactory
		{
		protected:  
			void defineMetatable(lua_State* L, string table_name, vector<luaL_Reg> fields);

		public: 
			LuaObjectFactory(){};
			~LuaObjectFactory(){};


			void defineMetatables(lua_State*);

			// Object Creation
			static int createAvatar(lua_State*);
			static int createCamera(lua_State*);
			static int createGUIButton(lua_State*);
			static int createGUIContext(lua_State*);
		}; 
	}
}
