#pragma once

#include <Vector2.h>
#include "Keys.h"
#include <queue>

namespace Pro {

	class Mouse { 
		static std::queue<KEY> mouse_keys;

	public:
		static Vector2<int> getMousePosition();
		static KEY getMouseKey();
		static bool hasMouseKey();

	};
}