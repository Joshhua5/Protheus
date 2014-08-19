#include "Scene.h"
using namespace Pro;
using namespace GameObject;
using namespace std;
using namespace Math;

Scene::Scene(lua_State* L) :
EntityContainer(),
CameraContainer()
{
	lua_state = L;
}

void Scene::update(){
}

// returns a list of entities found on a tile
vector<Entity*>  Scene::pollTile(Vector2& tile_position){
	vector<Entity*> list;
	for each(const auto obj in m_dynamic_entities)
		if (obj.second->getPosition() == tile_position)
			list.push_back(obj.second);

	for each(const auto obj in m_static_entities)
		if (obj.second->getPosition() == tile_position)
			list.push_back(obj.second);

	return list;
}

Map* Scene::getMap(){
	return tileData;
}

int Scene::lUpdate(lua_State* L){
	const auto s = Util::luaP_touserdata<Scene>(L, 1);
	s->update();
	return 0;
}