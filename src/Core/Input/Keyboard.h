#pragma once


#include <Vector2.h>
#include "UserPointers.h"
#include <utility>
#include <queue>
#include "Keys.h"

namespace Pro {
	namespace Input {
		class Keyboard {
			std::queue<std::pair<KEY, KEY>> keyboard_key_;
			GLFWwindow* window_;
		public:
			bool hasKey();
			// returns <modifier, key>
			std::pair<KEY, KEY> PollKey();
			KEY_PRESSED IsKeyDown(KEY);

			// depreciated
			void AttachWindow(GLFWwindow*);
		};
	}
}