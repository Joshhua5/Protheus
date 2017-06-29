#pragma once

#include "CAudioData.h"

namespace Pro{
	namespace Audio{
		class IAudioEffect
		{
		public:
			virtual CAudioData process(CAudioData) = 0;
			virtual void process(CAudioData&) = 0;
		};
	}
}
