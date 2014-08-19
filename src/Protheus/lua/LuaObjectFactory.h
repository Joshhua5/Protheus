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

#include "lib\lua.hpp"
#include "..\GameObjects.h"
#include "..\util\LuaUtils.h"
#include "..\component\CScriptable.h"
#include "..\component\Name.h"
#include "..\containers\SceneContainer.h"
#include "..\gui\GraphicalUI.h"

using namespace std;

namespace Pro{
	namespace Lua{
		class CLua;

		class LuaObjectFactory
		{
		public:
			LuaObjectFactory(lua_State** L){
				*L = luaL_newstate();
				// define luaP_state for global access
				Util::luaP_state(*L);
				const luaL_Reg globalFunctions[] = { 
					{ "scene_create", &LuaObjectFactory::createScene }
				};

				for each(const auto i in globalFunctions)
					lua_register(*L, i.name, i.func);
			};
			LuaObjectFactory(){}
			~LuaObjectFactory(){}

			// Object Creation
			static int createScene(lua_State* L);
			//static int createAvatar(lua_State*);
			//static int createCamera(lua_State*);
			static int createGUIButton(lua_State*);
			//static int createGUIContext(lua_State*);
		};
	}
}
