

#include "Entity.h"

using namespace Pro;
using namespace GameObject;

Entity::Entity(const std::string& name) : CGUID(name), ActiveState(), Position(){}

Entity::Entity() : CGUID(), ActiveState(), Position(){}


Entity::~Entity()
{
} 