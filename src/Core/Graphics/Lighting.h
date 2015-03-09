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
		//! Stores data about a specific point of light
		struct LightPoint {
			Vector3<float> position;
			Vector3<float> color;
			float attenuation, intensity;
		};

		//! A LightPoint with added directional data
		struct LightDirectional : public LightPoint {
			Vector3<float> direction_;
		};

		//! Container for a scene's lighting
		class Lighting {
			std::vector<LightPoint> light_points_;
			std::vector<LightDirectional> light_directional_;

			Vector3<float> ambient_color_;
		public:

			//! Binds the Lighting data to the @program
			void BindLights(Program& program);

			//! Attachs a LightPoint to the lighting system, making a copy and returns
			//! reference to the attached LightPoint
			LightPoint&  AttachLight(const LightPoint&);
			 
			//! Sets the ambient lighting
			void SetAmbient(const Vector3<float>& ambient); 
		};
	}
}