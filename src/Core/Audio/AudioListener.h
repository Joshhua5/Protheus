#pragma once

#include <Vector3.h>
#include "openal\al.h"

namespace Pro{
	namespace Audio{ 

		class CAudioListener
		{
		private:
			static unsigned active_listener_id; 
			int listener_id;

			Vector3<float> position, orientation, velocity;
			float gain;

			unsigned& getCount();

		public:
			CAudioListener();
			~CAudioListener();
			CAudioListener(const CAudioListener&);
			CAudioListener(CAudioListener&&);
			CAudioListener& operator=(const CAudioListener&);
			CAudioListener& operator=(CAudioListener&&);

			void setPosition(const Vector3<float>& v);

			void setOrientation(const Vector3<float>& v);

			void setVelocity(const Vector3<float>& v);

			// Value between 1 and 0 with 1 being unattenuated
			void setGain(float v);

			void setActive() const;
		};
	} 
}


