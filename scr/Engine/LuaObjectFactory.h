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
#include "CScriptable.h"
#include "ClassFlags.h"
#include "Name.h"
#include "GraphicalUI.h"

using namespace std;

namespace Pro{
	namespace Lua{ 
		class CLua;

		class LuaObjectFactory
		{ 
		public: 
			LuaObjectFactory(){ 
				const luaL_Reg globalFunctions [] = {
						{ "avatar_create", &LuaObjectFactory::createAvatar },
						{ "camera_create", &LuaObjectFactory::createCamera }
				};

				for each(auto i in globalFunctions)
					lua_register(Lua::lua_state, i.name, i.func);
			}; 
			~LuaObjectFactory(){}
 
			// Object Creation
			static int createAvatar(lua_State*);
			static int createCamera(lua_State*);
			static int createGUIButton(lua_State*);
			static int createGUIContext(lua_State*);
		}; 
	}
}
