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
			void move(float x, float y);
			float hypotenuse();

			//Vector2 operator=(Vector2&&);
			//Vector2 operator=(const Vector2&);
			//Vector2 operator=(Vector2);
			Vector2 operator=(SDL_Point&);   
			bool operator==(Vector2&);
			void operator+=(Vector2&);
			Vector2 operator-(Vector2&);

			SDL_Point toSDL();

			// LUA Functions

			static int lCreate(lua_State* L){ 
				if(lua_isnumber(L, 1) && lua_isnumber(L, 2))
					Util::luaP_newobject(L, new Vector2(luaP_tofloat(L, 1), luaP_tofloat(L, 2)));
				else 
					Util::luaP_newobject(L, new Vector2(0, 0));
				return 1;
			}
			static int lContains(lua_State*);
			static int lHypotenuse(lua_State*);
			static int lMove(lua_State*);
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
				fields.push_back({ "contains", &T::lContains });
				fields.push_back({ "hypotenuse", &T::lHypotenuse });
				fields.push_back({ "move", &T::lMove });
				fields.push_back({ "getX", &T::lGetX });
				fields.push_back({ "getY", &T::lGetY });
				fields.push_back({ "setX", &T::lSetX });
				fields.push_back({ "setY", &T::lSetY });
				fields.push_back({ "getXY", &T::lGetXY });
				fields.push_back({ "setXY", &T::lSetXY });
			}
		};
	}
}
