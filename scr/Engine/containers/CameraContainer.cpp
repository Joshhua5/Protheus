#include "CameraContainer.h"

using namespace Pro::GameObject;

namespace Pro{
	DataEntity* CameraContainer::getCamera(game_id i){
		return stored_cameras.at(i);
	}
	DataEntity* CameraContainer::getActiveCamera(){
		return active_camera;
	}
	void CameraContainer::setActiveCamera(game_id i){
		active_camera = getCamera(i);
	}
	void CameraContainer::addCamera(DataEntity* e){
		stored_cameras.insert({ e->getGUID(), e });
		if (active_camera == nullptr)
			active_camera = e;
	}
	void CameraContainer::destoryCamera(game_id i){
		stored_cameras.erase(i);
	}
}