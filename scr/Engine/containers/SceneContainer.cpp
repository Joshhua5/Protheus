#include "SceneContainer.h"

using namespace Pro;

void SceneContainer::addScene(Scene::Scene* scene){
	if (active_scene == nullptr)
		active_scene = scene;
	stored_scenes.insert(make_pair(scene->getGUID(), scene));
}

Scene::Scene* SceneContainer::getScene(uint32 id){
	return stored_scenes.at(id);
}

Scene::Scene* SceneContainer::getActiveScene(){
	return active_scene;
}

void SceneContainer::setActiveScene(uint32 id){
	active_scene = getScene(id);
}

void SceneContainer::destoryScene(uint32 id){
	stored_scenes.erase(id);
}

int SceneContainer::lAddScene(lua_State* L){
	Scene::Scene* s = Util::luaP_touserdata<Scene::Scene>(L, -1);
	SceneContainer* sc = Util::luaP_touserdata<SceneContainer>(L, -2);
	sc->addScene(s);
	return 0;
}

int SceneContainer::lGetActiveScene(lua_State* L){ 
	SceneContainer* sc = Util::luaP_touserdata<SceneContainer>(L, -1);
	Util::luaP_newuserdata<Scene::Scene>(L, sc->getActiveScene()); 
	return 1;
}

int SceneContainer::lGetScene(lua_State* L){
	SceneContainer* sc = Util::luaP_touserdata<SceneContainer>(L, -2);
	Util::luaP_newuserdata<Scene::Scene>(L, sc->getScene(static_cast<uint32>(lua_tonumber(L, -1))));
	return 1;
}

int SceneContainer::lDestoryScene(lua_State* L){
	SceneContainer* sc = Util::luaP_touserdata<SceneContainer>(L, -2);
	sc->destoryScene(static_cast<uint32>(lua_tonumber(L, -1)));
	return 0;
}

int SceneContainer::lSetActiveScene(lua_State* L){ 
	SceneContainer* sc = Util::luaP_touserdata<SceneContainer>(L, -2);
	sc->setActiveScene(static_cast<uint32>(lua_tonumber(L, -1)));
	return 0;
}
