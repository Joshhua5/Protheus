#include "Scene.h" 
namespace Pro{
	namespace Scene{

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

		bool Scene::loadSceneData(const std::string& path, const std::string& data){
			// Load Tile Data
			tileData = new Map();
			tileData->loadLevel(path, data);
			return 0;
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

		int Scene::lLoadScene(lua_State* L){
			Scene* s = Util::luaP_touserdata<Scene>(L, 1);
			s->loadSceneData(lua_tostring(L, 1), lua_tostring(L, 2));
			return 0;
		} 	

		int Scene::lUpdate(lua_State* L){
			Scene* s = Util::luaP_touserdata<Scene>(L, 1);
			s->update();
			return 0;
		}
	}
}


