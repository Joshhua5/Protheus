
#include "CameraContainer.h"


namespace Pro{
	Camera* CameraContainer::getCamera(uint32 i){
		return stored_cameras.at(i);
	}
	Camera* CameraContainer::getActiveCamera(){
		return active_camera;
	}
	void CameraContainer::setActiveCamera(uint32 i){
		active_camera = getCamera(i);
	}
	void CameraContainer::addCamera(Camera* e){
		stored_cameras.insert({ e->getGUID(), e });
		if (active_camera == nullptr)
			active_camera = e;
	}
	void CameraContainer::destoryCamera(uint32 i){
		stored_cameras.erase(i);
	}
}
