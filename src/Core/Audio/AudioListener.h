#pragma once

#include <Vector3.h>
#include <al.h>

namespace Pro{
	namespace Audio{
		using namespace Math;

		class CAudioListener
		{
		public:
			void setPosition(Vector3& v){ alListener3f(AL_POSITION, v.x, v.y, v.z); }
			void setOrientation(Vector3& v){ alListener3f(AL_ORIENTATION, v.x, v.y, v.z); }
			void setVelocity(Vector3& v) { alListener3f(AL_VELOCITY, v.x, v.y, v.z); } 

			// Value between 1 and 0 with 1 being unattenuated
			void setGain(float v) { alListenerf(AL_GAIN, v); }
		};
	}

}


