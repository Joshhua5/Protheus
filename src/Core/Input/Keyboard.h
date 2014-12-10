#pragma once

#include "..\Graphics\extern\glfw3.h"

#include <Vector2.h>
#include "UserPointers.h"
#include <utility>
#include <queue>
#include "Keys.h"
 
namespace Pro { 



	class Keyboard {
		std::queue<std::pair<KEY, KEY>> keyboard_key; 
		GLFWwindow* window;
	public: 
		bool hasKey();
		// returns <modifier, key>
		std::pair<KEY, KEY> poolKey();
		bool isKeyDown(KEY);

		// depreciated
		void attachWindow(GLFWwindow*);
	};
}