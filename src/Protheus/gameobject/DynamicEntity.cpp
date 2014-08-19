#include "DynamicEntity.h"

using namespace Pro;
using namespace GameObject;
using namespace Serializer;

DynamicEntity::DynamicEntity(const string& name) : Entity(name){}
DynamicEntity::DynamicEntity() : Entity(){}

const ClassDefinition DynamicEntity::getDefinition(){
	auto pointer = new DynamicEntity();
	ClassDefinition def(pointer);

	// Lua Function
	def.addMember(
		"lua_update",
		pointer->getFunction(),
		sizeof(char) * 32);

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