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
#include <memory>

namespace Pro{
	namespace Math{

		struct alignas(16) Vector2{
			float x, y;

			// Copy constructor
			Vector2(const Vector2&);
			Vector2& operator=(const Vector2& vec){
				x = vec.x;
				y = vec.y;
				return *this;
			}
			// Move Constructor
			Vector2(Vector2&&);
			Vector2& operator=(Vector2&& vec){
				x = std::move(vec.x);
				y = std::move(vec.y);
				return *this;
			}

			Vector2(){};

			Vector2(double, double);
			Vector2(unsigned long long, unsigned long long);
			Vector2(unsigned int, unsigned int);
			Vector2(float, float);
			Vector2(int, int);

			bool contains(float) const;
			void move(float _x, float _y);
			float length() const;
			Vector2 normalize() const;

			Vector2& operator=(float);

			bool operator==(const Vector2& p){
				return (x == p.x && y == p.y) ? true : false;
			}


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

			 
		};
	}
}
