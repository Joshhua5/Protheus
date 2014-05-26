#include "Camera.h"

using namespace Pro;
using namespace Scene;

Camera::Camera(const std::string& name) : CGUID(name){ 
}

Camera::Camera()
{ 
}

Camera::~Camera()
{
}

void Camera::move(Math::Vector2& v){
	position += v;
}
   
int Camera::lMove(lua_State* L){
	Camera* p = *(static_cast<Camera**>(lua_touserdata(L, 1)));
	p->move(Math::Vector2(
		static_cast<int>(lua_tonumber(L, 2)),
		static_cast<int>(lua_tonumber(L, 3))
		));
	return 0;
}