/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.

*************************************************************************/

#include "..\util\LuaUtils.h"

#ifdef __SSE
#include <xmmintrin.h>
#endif

#pragma once
namespace Pro{
	namespace Math{
		struct Vector3{
			float x, y, z;
			  
			Vector3();

			// LUA Functions
			 
			static int lGetX(lua_State*);
			static int lGetY(lua_State*);
			static int lGetZ(lua_State*); 
			static int lSetX(lua_State*);
			static int lSetY(lua_State*);
			static int lSetZ(lua_State*);
			static int lGetXYZ(lua_State*);
			static int lSetXYZ(lua_State*);

			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "vector3_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){ 
				fields.push_back({ "getX", (lua_CFunction)&T::lGetX });
				fields.push_back({ "getY", (lua_CFunction)&T::lGetY });
				fields.push_back({ "getZ", (lua_CFunction)&T::lGetZ });
				fields.push_back({ "setX", (lua_CFunction)&T::lSetX });
				fields.push_back({ "setY", (lua_CFunction)&T::lSetY });
				fields.push_back({ "setZ", (lua_CFunction)&T::lSetZ });
				fields.push_back({ "getXYZ", (lua_CFunction)&T::lGetXYZ });
				fields.push_back({ "setXYZ", (lua_CFunction)&T::lSetXYZ }); 
			}
		};
	} 
}

