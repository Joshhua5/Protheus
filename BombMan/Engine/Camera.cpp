#include "Camera.h"

namespace Pro{

	Camera::Camera()
	{
	}


	Camera::~Camera()
	{
	}

	SDL_Rect Camera::getPosition(){
		return position;
	}
	void Camera::move(int x, int y){
		position.x = x;
		position.y = y;
	}

	SDL_Rect* Camera::getDimensions(){
		return &position;
	}


	  
	void Camera::setDimensions(SDL_Rect _rect){
		position = _rect;
	}
	void Camera::setDimensions(int x, int y, int w, int h){
		position.x = x;
		position.y = y;
		position.w = w;
		position.h = h; 
	}
}