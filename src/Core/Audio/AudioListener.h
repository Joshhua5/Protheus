#pragma once

#include <Vector3.h>
#include "openal\al.h"

namespace Pro{
	namespace Audio{ 
		using namespace Math;
		class CAudioListener
		{ 
			static unsigned active_listener_id; 
			int listener_id;

			Vector3<float> position_, orientation_, velocity_;
			float gain_;

			unsigned& getCount();

		public:
			CAudioListener();
			~CAudioListener();
			CAudioListener(const CAudioListener&);
			CAudioListener(CAudioListener&&);
			CAudioListener& operator=(const CAudioListener&);
			CAudioListener& operator=(CAudioListener&&);

			void position(const Vector3<float>& v);

			void setOrientation(const Vector3<float>& v);

			void velocity(const Vector3<float>& v);

			const Vector3<float>& velocity() const { return velocity_; }

			// Value between 1 and 0 with 1 being unattenuated
			void setGain(float v);

			void setActive() const;
		};
	} 
}


