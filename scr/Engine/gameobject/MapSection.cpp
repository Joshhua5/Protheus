
#include "MapSection.h"

using namespace Pro;
using namespace GameObject;


short MapSection::tileAt(Math::Vector2& v){
	char* ch = contains(v);
	if (ch != nullptr)
		return *ch;
	return 0;
}

short* MapSection::contains(Math::Vector2& v){
	if (Math::Vector4(position, dimensions).contains(v))
			return &data[static_cast<int>(v.x)][static_cast<int>(v.y)];
	return nullptr;
}
 

void MapSection::setData(std::vector<std::vector<short>> dat){
	data = dat;
}

bool MapSection::visible(Camera* cam){ 
	// need to check if the camera can see the MapSection
	// top bottom check  

	// Convert the position and dimensions into a Vector4 from 2(Vector2)
	return Math::Vector4(position, dimensions).overlaps(
		   Math::Vector4(cam->getPosition(), cam->getDimensions())
		); 
}

std::vector<std::vector<short>> MapSection::getData(){
	return data;
}