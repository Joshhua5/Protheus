#include "MapSection.h"

using namespace Pro;
using namespace GameObject;
using namespace Math;

short MapSection::tileAt(Math::Vector2& v){
	short* ch = contains(v);
	if (ch != nullptr)
		return *ch;
	return 0;
}

short* MapSection::contains(Math::Vector2& v){
	if (Vector4(position, dimensions).contains(v))
		return &data[static_cast<int>(v.x)][static_cast<int>(v.y)];
	return nullptr;
}

void MapSection::setData(std::vector<std::vector<short>> dat){
	data = dat;
}

bool MapSection::visible(DataEntity& cam){
	// need to check if the camera can see the MapSection
	// top bottom check

	auto v4 = Vector4(
		*cam.getData<Vector2>("position"),
		*cam.getData<Vector2>("dimension")
		);

	// Convert the position and dimensions into a Vector4 from 2(Vector2)
	return Vector4(position, dimensions).overlaps(move(v4));
}

std::vector<std::vector<short>> MapSection::getData(){
	return data;
}