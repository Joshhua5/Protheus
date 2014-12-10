#pragma once
  

#include <Vector2.h>
#include "Keys.h"
#include "UserPointers.h"
#include <queue>

namespace Pro {

	class Mouse { 
		std::queue<std::pair<KEY, KEY>> mouse_keys;
		GLFWwindow* window;
	public:
		Vector2<int> getMousePosition();
		
		template<typename T>
		void getMousePosition(Vector2<T>& vector);

		template<typename T>
		void getMousePosition(T* x, T* y);
		  
		std::pair<KEY, KEY> getMouseKey();
		bool hasMouseKey();
		 
		//Depreciated 
		void attachWindow(GLFWwindow*); 
	};

	template<typename T>
	void Mouse::getMousePosition(Vector2<T>& vector) {
		getMousePosition<T>(vector.x, vector.y);
	}

	template<typename T>
	void Mouse::getMousePosition(T* x, T* y) {
		double _x, _y;
		glfwGetCursorPos(window, &_x, &_y);
		if (x != nullptr)
			*x = static_cast<T>(_x);
		if (y != nullptr)
			*y = static_cast<T>(_y); 
	}

}