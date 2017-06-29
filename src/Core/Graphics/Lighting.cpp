#include "Lighting.h"

using namespace Pro; 
using namespace Util;
using namespace Graphics;

void Lighting::BindLights(Program& program) {  
	Buffer points(light_points_.size() * sizeof(float) * 8);
	BufferWriter writer(&points);

	for (auto& light : light_points_) {
		writer.WriteElements<float>(&light.position.x, 3);
		writer.WriteElements<float>(&light.color.x, 3);
		writer.Write<float>(light.attenuation);
		writer.Write<float>(light.intensity);
	}  

	for (auto& light : light_directional_) {
		writer.WriteElements<float>(&light.position.x, 3);
		writer.WriteElements<float>(&light.color.x, 3); 
		writer.WriteElements<float>(&light.direction_.x, 3);
		writer.Write<float>(light.attenuation);
		writer.Write<float>(light.intensity);
	}

	program.SetUniform("light_ambient", ambient_color_); 
	program.SetUniform("light_point_count", (int)light_points_.size());
	program.SetUniform("light_points", points.data<float>(), (int)light_points_.size() * 8);
	program.SetUniform("light_directional_count", (int)light_directional_.size());
	program.SetUniform("light_directional",
		points.data<float>() + (light_points_.size() * 8), (int)light_directional_.size() * 11);
}

LightPoint& Lighting::AttachLight(const LightPoint& point) {
	light_points_.push_back(point);
	return light_points_.back();
}

void Lighting::SetAmbient(const Vector3<float>& ambient) {
	ambient_color_ = ambient;
}