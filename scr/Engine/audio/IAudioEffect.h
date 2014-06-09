#pragma once
 
#include "CAudioTrack.h"

namespace Pro{
	namespace Audio{
		class IAudioEffect
		{
		public:
			CAudioTrack process(CAudioTrack);
			void process(CAudioTrack&);
		};
	}
}
