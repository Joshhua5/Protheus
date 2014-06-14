
#include "Position.h"

using namespace Pro;
using namespace Component;

Position::Position()
{
}


Position::~Position()
{
}

Math::Vector2 Position::getPosition(){
	return position;
}
Math::Vector2* Position::pGetPosition(){
	return &position;
}
void Position::setPosition(Math::Vector2& p){
	position = p;
}
 