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
 
#include <cmath>
#include "..\util\LuaUtils.h"

namespace Pro{
	namespace Math{

		struct alignas(16) Vector2{ 
			float x, y;

			// Copy constructor
			Vector2(const Vector2&);
			Vector2& operator=(const Vector2&); 
			// Move Constructor
			Vector2(Vector2&&);
			Vector2& operator=(Vector2&&);

			Vector2(){};

			Vector2(double, double);
			Vector2(unsigned long long, unsigned long long);
			Vector2(unsigned int, unsigned int);
			Vector2(float, float);
			Vector2(int, int);

			bool contains(float);
			void move(float _x, float _y);
			float length();
			Vector2 normalize();

			Vector2& operator=(float);
			bool operator==(const Vector2&); 

			void operator+=(float);
			void operator+=(Vector2&);
			Vector2 operator+(Vector2&);
			Vector2 operator+(float);

			Vector2 operator-(float);
			Vector2 operator-(Vector2&);
			void operator-=(float);
			void operator-=(Vector2&);

			Vector2 operator/(float);
			Vector2 operator/(Vector2&);
			void operator/=(float);
			void operator/=(Vector2&);

			Vector2 operator*(float);
			Vector2 operator*(Vector2&);
			void operator*=(float);
			void operator*=(Vector2&);


			// LUA Functions

			static int lCreate(lua_State* L);
			static int lContains(lua_State*);
			static int lLength(lua_State*);
			static int lNormalize(lua_State*);
			static int lMove(lua_State*);
			static int lGetX(lua_State*);
			static int lGetY(lua_State*);
			static int lSetX(lua_State*);
			static int lSetY(lua_State*);
			static int lGetXY(lua_State*);
			static int lSetXY(lua_State*);

			// returns the Metatable's name assosiated with this object
			/*constexpr*/ static const char* lGetMetatable(){
				return "vector2_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "contains", &T::lContains });
				fields.push_back({ "length", &T::lLength });
				fields.push_back({ "normalize", &T::lNormalize });
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
