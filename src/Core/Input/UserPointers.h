#pragma once

#include <queue>

namespace Pro {   
	enum struct KEY;
	struct InputPointers {
		std::queue<std::pair<KEY, KEY>>* keyboard;
		std::queue<std::pair<KEY, KEY>>* mouse;
	};
}