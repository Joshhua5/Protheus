#include "CameraContainer.h"


namespace Pro{
	Scene::Camera* CameraContainer::getCamera(uint32 i){
		return stored_cameras.at(i);
	}
	Scene::Camera* CameraContainer::getActiveCamera(){
		return active_camera;
	}
	void CameraContainer::setActiveCamera(uint32 i){
		active_camera = getCamera(i);
	}
	void CameraContainer::addCamera(Scene::Camera* e){
		stored_cameras.insert({ e->getGUID(), e });
	}
	void CameraContainer::destoryCamera(uint32 i){
		stored_cameras.erase(i);
	}
}
