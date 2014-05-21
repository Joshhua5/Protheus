#include "SceneContainer.h"

using namespace Pro;
 
 
int SceneContainer::lSetActiveScene(lua_State* L){
	Util::luaP_registerstore(L, "ACTIVE_SCENE", Util::luaP_touserdata<Scene::Scene>(L, 2));
	return 0;
}