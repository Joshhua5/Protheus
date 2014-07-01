
#include "Scene.h" 
namespace Pro{
	namespace GameObject{

		Scene::Scene(lua_State* L) : 
			EntityContainer(),
			CameraContainer()
		{
			lua_state = L;
		}


		Scene::~Scene()
		{
		}

		void Scene::update(){

		}

		// returns a list of entities found on a tile
		std::vector<GameObject::Entity*>  Scene::pollTile(Math::Vector2& v){
			std::vector<GameObject::Entity*> list;
			for each(auto obj in stored_entities)
				if (obj.second->getPosition() == v)
						list.push_back(obj.second);
			return list;
		} 

		Map* Scene::getMap(){
			return tileData;
		}  

		int Scene::lUpdate(lua_State* L){
			Scene* s = Util::luaP_touserdata<Scene>(L, 1);
			s->update();
			return 0; 
		}
	}
}


