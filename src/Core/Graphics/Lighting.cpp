#include "Lighting.h"

using namespace Pro;
using namespace Util;

void Lighting::bindLights(GLuint program_id) {  
	CBuffer points(light_points.size() * sizeof(float) * 8);
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

	Program::setUniform(program_id, "light_ambient", ambientColor); 
	Program::setUniform(program_id, "light_point_count", light_points.size());
	Program::setUniform(program_id, "light_points", points.data<float>(), light_points.size() * 8);
	Program::setUniform(program_id, "light_directional_count", light_directional.size());
	Program::setUniform(program_id, "light_directional",
		points.data<float>() + (light_points.size() * 8), light_directional.size() * 11); 
}

void Lighting::attachLight(const LightPoint& point) { 
	light_points.push_back(point);
}


void Lighting::setAmbient(const Vector3<float>& ambient) {
	ambientColor = ambient;
}