/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:05:2014 Waring J.
*************************************************************************/
#pragma once
#include "lua\lua.hpp" 
#include "Avatar.h"
#include "Scene.h"
#include "LuaUtils.h"
#include "GraphicalUI.h"

namespace Pro{
	namespace Lua{
		class CLua;

		class LuaObjectFactory
		{
		protected: 
			void defineMetatable(lua_State*, const char* table_name, luaL_Reg* fields);
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
