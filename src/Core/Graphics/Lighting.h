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
			float attenuation_, intensity_;
		};

		struct LightDirectional : public LightPoint {
			Vector3<float> direction_;
		};

		class Lighting {
			std::vector<LightPoint> light_points_;
			std::vector<LightDirectional> light_directional_;

			Vector3<float> ambient_color_;
		public:
			void BindLights(Program& program);
			LightPoint&  AttachLight(const LightPoint&);
			void SetAmbient(const Vector3<float>& ambient);

		};
	}
}