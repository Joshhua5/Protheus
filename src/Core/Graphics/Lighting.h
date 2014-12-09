#pragma once

#include <Vector3.h>
#include <Vector4.h>
#include <vector>
#include "Program.h"
#include <BufferWriter.h>

		//ambient 
		//{attenuation, intensity, position, direction, color}
		//{attenuation, intensity, position, color}
		//direction_count
		//point_count

namespace Pro {
	struct LightPoint{
		Vector3<float> position;
		Vector3<float> color;
		float attenuation, intensity;
	};

	struct LightDirectional {
		Vector3<float> position;
		Vector3<float> color;
		Vector3<float> direction;
		float attenuation, intensity;
	};
	  
	class Lighting {
		std::vector<LightPoint> light_points;
		std::vector<LightDirectional> light_directional;
		
		Vector3<float> ambientColor;
	public:
		void bindLights(GLuint program_id);
		void attachLight(const LightPoint&);  
		void setAmbient(const Vector3<float>& ambient);
	};
}