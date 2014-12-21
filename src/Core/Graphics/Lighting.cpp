#include "Lighting.h"

using namespace Pro; 
using namespace Util;
using namespace Graphics;

void Lighting::bindLights(Program& program) {  
	Buffer points(light_points.size() * sizeof(float) * 8);
	BufferWriter writer(&points);

	for (auto& light : light_points) {
		writer.write_elements<float>(&light.position.x, 3);
		writer.write_elements<float>(&light.color.x, 3);
		writer.write<float>(light.attenuation);
		writer.write<float>(light.intensity);
	}  

	for (auto& light : light_directional) {
		writer.write_elements<float>(&light.position.x, 3);
		writer.write_elements<float>(&light.color.x, 3); 
		writer.write_elements<float>(&light.direction.x, 3);
		writer.write<float>(light.attenuation);
		writer.write<float>(light.intensity);
	}

	program.setUniform("light_ambient", ambientColor); 
	program.setUniform("light_point_count", light_points.size());
	program.setUniform("light_points", points.data<float>(), light_points.size() * 8);
	program.setUniform("light_directional_count", light_directional.size());
	program.setUniform("light_directional",
		points.data<float>() + (light_points.size() * 8), light_directional.size() * 11); 
}

LightPoint& Lighting::attachLight(const LightPoint& point) {
	light_points.push_back(point);
	return light_points.back();
}


void Lighting::setAmbient(const Vector3<float>& ambient) {
	ambientColor = ambient;
}