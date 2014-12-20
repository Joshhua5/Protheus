#pragma once

#include "CAudioData.h"

namespace Pro{
	namespace Audio{
		class IAudioEffect
		{
		public:
			CAudioData process(CAudioData);
			void process(CAudioData&);
		};
	}
}
