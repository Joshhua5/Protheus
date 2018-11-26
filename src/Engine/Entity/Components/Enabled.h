#pragma once

#include "../Component.h"

namespace Pro {
	namespace ECS {
		struct Enabled : Component { 
			constexpr static unsigned ID() {
				FowlerNollVoHash(__PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__));
			} 

			bool enabled;

			Enabled(bool value = true) {
				enabled = value;
			}
		};
	}
}