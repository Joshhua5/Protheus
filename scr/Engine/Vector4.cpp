/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.

*************************************************************************/

#include "Vector4.h"

using namespace Pro;
using namespace Math;

Vector4::Vector4(SDL_Rect& p) : Vector4(
	static_cast<float>(p.x),
	static_cast<float>(p.y),
	static_cast<float>(p.w),
	static_cast<float>(p.h))
{}

Vector4::Vector4(int x, int y, int z, int w) : Vector4(
	static_cast<float>(x),
	static_cast<float>(y),
	static_cast<float>(z),
	static_cast<float>(w))
{}

Vector4::Vector4(float _x, float _y, float _z, float _w){
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

Vector4::Vector4()
{
	x = y = z = w = 0;
}


Vector4::~Vector4()
{
}

Vector4 Vector4::operator=(const SDL_Rect& p){ return Vector4(p.x, p.y, p.w, p.h); }

SDL_Rect Vector4::toSDL(){
	SDL_Rect o;
	o.x = static_cast<int>(x);
	o.y = static_cast<int>(y);
	o.w = static_cast<int>(z);
	o.h = static_cast<int>(w);
	return o;
}