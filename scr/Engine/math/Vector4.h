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

#define __SSE
#ifdef __SSE
#include <xmmintrin.h>
#endif
 
#include <SDL.h>
#include "Vector2.h"
namespace Pro{
	namespace Math{

#ifdef _MSC_VER
		__declspec(align(16))
#endif
		struct Vector4{

			float x, y, z, w;
			
			Vector4(SDL_Rect& p);
			Vector4(const Vector4&);
			Vector4(Vector2&, Vector2&);
			Vector4(double, double, double, double);
			Vector4(float x, float y, float z, float w);
			Vector4(int x, int y, int z, int w);
			Vector4();
			~Vector4();
			
			// z and w are width and height in that order.
			bool contains(Vector2&);
			bool overlaps(Vector4&);

			Vector4 operator+(Vector4&);
			Vector4 operator-(Vector4&);
			Vector4 operator*(Vector4&);
			Vector4 operator/(Vector4&);

			Vector4 operator=(const SDL_Rect&);
			Vector4 operator=(Vector4&);

			void operator+=(Vector4&);
			void operator-=(Vector4&);
			void operator*=(Vector4&);
			void operator/=(Vector4&);
			SDL_Rect toSDL();

			// Lua Functions

			static int lGetX(lua_State*);
			static int lGetY(lua_State*);
			static int lGetZ(lua_State*);
			static int lGetW(lua_State*);
			static int lSetX(lua_State*);
			static int lSetY(lua_State*);
			static int lSetZ(lua_State*);
			static int lSetW(lua_State*);
			static int lGetXYZW(lua_State*);
			static int lSetXYZW(lua_State*);
			  
			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "vector4_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){ 
				fields.push_back({ "getX", (lua_CFunction)&T::lGetX });
				fields.push_back({ "getY", (lua_CFunction)&T::lGetY });
				fields.push_back({ "getZ", (lua_CFunction)&T::lGetZ }); 
				fields.push_back({ "getW", (lua_CFunction)&T::lGetW });
				fields.push_back({ "setX", (lua_CFunction)&T::lSetX });
				fields.push_back({ "setY", (lua_CFunction)&T::lSetY });
				fields.push_back({ "setZ", (lua_CFunction)&T::lSetZ }); 
				fields.push_back({ "setW", (lua_CFunction)&T::lSetW });
				fields.push_back({ "getXYZW", (lua_CFunction)&T::lGetXYZW });
				fields.push_back({ "setXYZW", (lua_CFunction)&T::lSetXYZW });
			}
		}; 
	} 
}
