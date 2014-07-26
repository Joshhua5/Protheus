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

#define SDL_RectCreate(v) SDLP_RectCreate(v.x, v.y, v.z, v.w)
#define SDL_ColorCreate(v)  SDLP_ColorCreate((Uint8) v.x, (Uint8) v.y, (Uint8) v.z, (Uint8) v.w)

namespace Pro{
	namespace Math{
#ifdef _MSC_VER
		__declspec(align(16))
#endif
		struct Vector4{
			float x, y, z, w;

			// Copy Constructor
			Vector4(const Vector4&);
			// Move Constructor
			Vector4(Vector4&&);

			Vector4(){}
			 
			Vector4(const Vector2& xy, const Vector2& zw);
			//Vector4(Vector2& xy, Vector2& zw);
			Vector4(double x, double y, double z, double w);
			Vector4(float x, float y, float z, float w);
			Vector4(int x, int y, int z, int w);

			// z and w are width and height in that order.
			bool contains(const Vector2&) const;
			bool overlaps(const Vector4&) const;

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
