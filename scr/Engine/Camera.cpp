#include "Camera.h"

namespace Pro{

	Camera::Camera()
	{
	}


	Camera::~Camera()
	{
	}

	SDL_Point* Camera::getPosition(){
		return &position;
	}
	void Camera::move(int x, int y){
		position.x = x;
		position.y = y;
	}

	SDL_Point* Camera::getDimensions(){
		return &dimensions;
	}  
	void Camera::setDimensions( int w, int h){ 
		dimensions.x = w;
		dimensions.y = h;
	}
	void Camera::setPosition(int x, int y){
		position.x = x;
		position.y = y;
	}
	

	int Camera::lGetDimensions(lua_State* L){
		Camera* p = *(static_cast<Camera**>(lua_touserdata(L, 1)));
		SDL_Point* dim = p->getDimensions();
		lua_pushnumber(L, dim->x);
		lua_pushnumber(L, dim->y);
		return 2;
	}
	int Camera::lGetPosition(lua_State* L){
		Camera* p = *(static_cast<Camera**>(lua_touserdata(L, 1)));
		SDL_Point* dim = p->getPosition();
		lua_pushnumber(L, dim->x);
		lua_pushnumber(L, dim->y);
		return 2;
	}
	int Camera::lSetDimensions(lua_State* L){
		Camera* p = *(static_cast<Camera**>(lua_touserdata(L, 1)));
		p->setPosition(
			static_cast<int>(lua_tonumber(L, 2)),
			static_cast<int>(lua_tonumber(L, 3)));
		return 0;
	}
	int Camera::lSetPosition(lua_State* L){
		Camera* p = *(static_cast<Camera**>(lua_touserdata(L, 1)));
		p->setPosition(
			static_cast<int>(lua_tonumber(L, 2)),
			static_cast<int>(lua_tonumber(L, 3)));
		return 0;
	}
	int Camera::lMove(lua_State* L){
		Camera* p = *(static_cast<Camera**>(lua_touserdata(L, 1)));
		p->move(
			static_cast<int>(lua_tonumber(L, 2)),
			static_cast<int>(lua_tonumber(L, 3)));
		return 0;
	}
	 
}