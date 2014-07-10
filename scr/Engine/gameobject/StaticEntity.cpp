#include "StaticEntity.h"

using namespace Pro;
using namespace GameObject;
using namespace Component;
using namespace Serializer;
  
StaticEntity::StaticEntity(const string& name) : Entity(name){ }
StaticEntity::StaticEntity(){}

const ClassDefinition StaticEntity::getDefinition(){
	auto pointer = new StaticEntity();
	ClassDefinition def(pointer); 

	// Animation
	def.addMember(
		"animation",
		pointer->getAnimationName(),
		sizeof(char) * 32);
	// Texture
	def.addMember(
		"texture",
		pointer->getSpriteName(),
		sizeof(char) * 32);
	// Position
	def.addMember(
		"position",
		pointer->pGetPosition(),
		sizeof(Vector2));

	// Area
	def.addMember(
		"dimensions",
		pointer->pGetDimensions(),
		sizeof(Vector2));

	def.finish();
	return move(def);
}