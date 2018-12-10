#pragma once

#include "../Component.h"

namespace Pro {
	namespace ECS {
		struct Enabled : Component { 
			constexpr static unsigned ID() {
				return FowlerNollVoHash(__FUNCSIG__, sizeof(__FUNCSIG__));
			}  
			bool enabled;

			Enabled() {
				enabled = true;
			}
		};
	}
}