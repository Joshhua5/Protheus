#include "Scene.h" 
namespace Pro{

	Scene::Scene()
	{
	}


	Scene::~Scene()
	{
	}

	bool Scene::loadSceneData(const std::string& path, const std::string& data){
		// Load Tile Data
		tileData = new Map();
		tileData->loadLevel(path, data);
		return true;
	} 

	bool Scene::destoryEntity(ID id){
		if (entities.erase(id) == 0)
			return false;
		return true;
	} 

	// returns a list of entities found on a tile
	std::vector<Entity*>  Scene::pollTile(unsigned int x, unsigned int y){
		std::vector<Entity*> list;
		for each(auto obj in entities)
			if (obj.second.getPosX() == x)
				if (obj.second.getPosY() == y)
					list.push_back(&obj.second);
		return list;
	}
	// returns a list of entities found on a tile
	std::vector<Entity*>  Scene::pollTile(SDL_Rect pos){
		std::vector<Entity*> list;
		for each(auto obj in entities)
			if (obj.second.getPosX() == pos.x)
				if (obj.second.getPosY() == pos.y)
					list.push_back(&obj.second);
		return list;
	}

	Entity*  Scene::getEntity(ID id){
		return &entities.at(id);
	} 
	// allows the id to be retrieved via the name
	void Scene::addEntity(Entity entity, ID _id){ 
		entities.insert({ _id, entity }); 
	}

	Map* Scene::getMap(){
		return tileData;
	}
	// id must be managed, if lost the entity can't be retrieved. 

	std::unordered_map<ID, Entity>* Scene::getEntities(){
		return &entities;
	}

	Camera* Scene::getCamera(ID _id){
		return cameras.at(_id);
	}
	Camera* Scene::getActiveCamera(){
		return activeCamera;
	}
	void Scene::setActiveCamera(ID _id){
		activeCamera = cameras.at(_id);
	}
	void Scene::addCamera(ID _id, Camera _cam){
		// Make a copy of Camera
		Camera* cam = new Camera();
		*cam = _cam;
		cameras.insert({ _id, cam });
	}
}


