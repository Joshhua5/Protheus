#include "Vector2.h"

using namespace Pro;
using namespace Math;


Vector2::Vector2(int x, int y) : Vector2(static_cast<float>(x), static_cast<float>(y)) {}
Vector2::Vector2(unsigned int x, unsigned int y) : Vector2(static_cast<float>(x), static_cast<float>(y)) {}

Vector2::Vector2(float _x, float _y){
	x = _x;
	y = _y;
}


Vector2::Vector2()
{
	x = 0;
	y = 0;
}


Vector2::~Vector2(){}

Vector2 Vector2::operator=(SDL_Point& p){ return Vector2(p.x, p.y); }

SDL_Point Vector2::operator=(Vector2& p){
	SDL_Point o;
	o.x = static_cast<int>(p.x);
	o.y = static_cast<int>(p.y);
	return o;
}

bool Vector2::operator==(Vector2& p){
	if (x == p.x && y == p.y)
		return true;
	return false;
}

void Vector2::operator+=(Vector2& p){
	x += p.x;
	y += p.y;
}


bool Vector2::contains(float& p){
	if ((x > p && y < p) || (x < p && y > p))
		return true;
	return false;
}
