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
#include "ClassFlags.h"
#include "GraphicalUI.h"

using namespace std;

namespace Pro{
	namespace Lua{ 
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
