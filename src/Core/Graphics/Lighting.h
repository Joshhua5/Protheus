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
	namespace Graphics {
		using namespace Math;
		struct LightPoint {
			Vector3<float> position;
			Vector3<float> color;
			float attenuation, intensity;
		};

		struct LightDirectional : public LightPoint {
			Vector3<float> direction;
		};

		class Lighting {
			std::vector<LightPoint> light_points;
			std::vector<LightDirectional> light_directional;

			Vector3<float> ambientColor;
		public:
			void bindLights(Program& program);
			LightPoint&  attachLight(const LightPoint&);
			void setAmbient(const Vector3<float>& ambient);

		};
	}
}