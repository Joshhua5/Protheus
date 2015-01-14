#pragma once
#include "extern\cl.h"

namespace Pro {
	namespace OpenCL {
		class Event {
			cl_event _event;
		public:
			const cl_event& getEvent() const { return _event; }
		
		};
	}
}