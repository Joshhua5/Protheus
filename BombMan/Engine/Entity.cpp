#include "Entity.h"
namespace Pro{


	Entity::Entity()
	{
	}


	Entity::~Entity()
	{
	}

	int* Entity::getPos(){
		int* out = new int[2]{position.x, position.y};
		return out;
	}

	int Entity::getPosX(){
		return position.x;
	}
	int Entity::getPosY(){
		return position.y;
	}

	void Entity::setPosX(int value){
		position.x = value;
	}
	void Entity::setPosY(int value){
		position.y = value;
	}

	void Entity::setPos(int X, int Y){
		position.x = X;
		position.y = Y;
	}

	unsigned int Entity::getID(){
		return ID;
	}
	void Entity::setID(unsigned int value){
		ID = value;
	}

	SDL_Rect Entity::getPositionRect(){
		return position;
	}

	SDL_Rect* Entity::getPositionRectP(){
		return &position;
	}
}