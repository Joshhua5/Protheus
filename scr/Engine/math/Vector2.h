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

#include <SDL.h>
#include <cmath>
#include "..\util\LuaUtils.h"

namespace Pro{
	namespace Math{ 

		struct Vector2 {
			float x, y;

			// Copy constructor
			Vector2(const Vector2&);
			// Move Constructor
			Vector2(Vector2&&);

			Vector2(){};

			Vector2(SDL_Point);
			Vector2(double, double);
			Vector2(unsigned long long, unsigned long long);
			Vector2(unsigned int, unsigned int);
			Vector2(float, float);
			Vector2(int, int); 

			bool contains(float);
			float hypotenuse();

			Vector2 operator=(SDL_Point&);
			SDL_Point operator=(Vector2&);
			bool operator==(Vector2&);
			void operator+=(Vector2&); 
			Vector2 operator-(Vector2&);


			// LUA Functions

			static int lContains(lua_State*);
			static int lHypotenuse(lua_State*);
			static int lGetX(lua_State*);
			static int lGetY(lua_State*); 
			static int lSetX(lua_State*);
			static int lSetY(lua_State*);
			static int lGetXY(lua_State*);
			static int lSetXY(lua_State*);
			 
			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "vector2_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "contains", (lua_CFunction) &T::lContains });
				fields.push_back({ "hypotenuse", (lua_CFunction) &T::lHypotenuse });
				fields.push_back({ "getX", (lua_CFunction) &T::lGetX });
				fields.push_back({ "getY", (lua_CFunction) &T::lGetY });
				fields.push_back({ "setX", (lua_CFunction) &T::lSetX });
				fields.push_back({ "setY", (lua_CFunction) &T::lSetY });
				fields.push_back({ "getXY", (lua_CFunction) &T::lGetXY });
				fields.push_back({ "setXY", (lua_CFunction) &T::lSetXY });
			}
		}; 
	}
}


