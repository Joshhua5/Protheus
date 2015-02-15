#pragma once
   
#include <Vector2.h>
#include "Keys.h"
#include "UserPointers.h"
#include <queue>

namespace Pro {
	namespace Input {
		using namespace Math;
		class Mouse {
			std::queue<std::pair<KEY, KEY>> mouse_keys_;
			GLFWwindow* window_;
		public:
			Mouse();
			Mouse(GLFWwindow* window);

			Vector2<int> position();

			template<typename T>
			void position(Vector2<T>& vector);

			template<typename T>
			void position(T* x, T* y);

			std::pair<KEY, KEY> GetMouseKey();
			//! Returns true if new key events are avaliable
			bool HasMouseKey();
			 
			void AttachWindow(GLFWwindow*);
		};

		template<typename T>
		void Mouse::position(Vector2<T>& vector) {
			position<T>(vector.x, vector.y);
		}

		template<typename T>
		void Mouse::position(T* x, T* y) {
			double _x, _y;
			glfwGetCursorPos(window_, &_x, &_y);
			if (x != nullptr)
				*x = static_cast<T>(_x);
			if (y != nullptr)
				*y = static_cast<T>(_y);
		}
	}
}