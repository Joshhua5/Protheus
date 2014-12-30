#pragma once


#include <utility>
#include <queue>

namespace Pro {
	namespace Input {
		enum struct KEY;
		struct InputPointers {
			std::queue<std::pair<KEY, KEY>>* keyboard;
			std::queue<std::pair<KEY, KEY>>* mouse;
		};
	}
}