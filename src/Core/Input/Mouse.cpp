#include "Mouse.h"

using namespace Pro;
using namespace Input;

void mouse_callback(GLFWwindow* window, int key, int action, int mod) {
	static_cast<InputPointers*>(glfwGetWindowUserPointer(window))
		->mouse->push({ (KEY)mod, (KEY)key });
}

void Mouse::attachWindow(GLFWwindow* window) {
	glfwSetMouseButtonCallback(window, &mouse_callback);
	static_cast<InputPointers*>(glfwGetWindowUserPointer(window))->mouse = &mouse_keys;
	this->window = window;
}

Vector2<int> Mouse::getMousePosition() {  
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return Vector2<int>(static_cast<int>(x), static_cast<int>(y));
}

std::pair<KEY, KEY> Mouse::getMouseKey() {
	if (!hasMouseKey())
		return{ KEY::KEY_UNKNOWN, KEY::KEY_UNKNOWN };
	std::pair<KEY, KEY> k = std::move(mouse_keys.front());
	mouse_keys.pop();
	return k;
}

bool Mouse::hasMouseKey(){
	return !mouse_keys.empty();
}

