#include "MapSection.h"

using namespace Pro;
using namespace Scene;


char MapSection::tileAt(unsigned int x, unsigned int y){
	char* ch = contains(x, y);
	if (ch != nullptr)
		return *ch;
	return 0;
}

char* MapSection::contains(int x, int y){
	if (x > dimensions.x && dimensions.x + dimensions.w > x)
		if (y > dimensions.y && dimensions.x + dimensions.h > y)
			return &data[x][y];
	return nullptr;
}

SDL_Rect MapSection::getDimensions(){
	return dimensions;
}

void MapSection::setData(std::vector<std::vector<char>> dat){
	data = dat;
}

bool MapSection::visible(Camera* cam){
	Math::Vector2 pos = cam->getPosition();
	Math::Vector2 dim = cam->getDimensions();
#define d dimensions 
	// need to check if the camera can see the MapSection
	// top bottom check 
	if (d.x >= pos.x && d.x + d.w <= pos.x + dim.x){
		if (d.y + d.h <= pos.y && d.y >= pos.y)
			return true;
		if (d.y + d.h > pos.y && d.y + d.h < pos.y + dim.y)
			return true;
	}
	// check left and right
	else if (d.y > pos.y && d.y + d.h < pos.y + dim.y){
		if (d.x + d.w > pos.x && d.x + d.w < pos.x + dim.x)
			return true;
		if (d.x > pos.x + dim.x && d.x < pos.x + dim.x)
			return true;
	}
	return false;
#undef d
}

std::vector<std::vector<char>> MapSection::getData(){
	return data;
}