#include "Mouse.h"

using namespace Pro;
using namespace Input;


Mouse::Mouse() {
	window_ = nullptr;
}
Mouse::Mouse(GLFWwindow* window) {
	AttachWindow(window);
}

void mouse_callback(GLFWwindow* window, int key, int action, int mod) {
	static_cast<InputPointers*>(glfwGetWindowUserPointer(window))
		->mouse->push({ (KEY)mod, (KEY)key });
}

void Mouse::AttachWindow(GLFWwindow* window) {
	glfwSetMouseButtonCallback(window, &mouse_callback);
	static_cast<InputPointers*>(glfwGetWindowUserPointer(window))->mouse = &mouse_keys_;
	this->window_ = window;
}

Vector2<int> Mouse::position() {  
	double x, y;
	glfwGetCursorPos(window_, &x, &y);
	return Vector2<int>(static_cast<int>(x), static_cast<int>(y));
}

std::pair<KEY, KEY> Mouse::GetMouseKey() {
	if (!HasMouseKey())
		return{ KEY::KEY_UNKNOWN, KEY::KEY_UNKNOWN };
	std::pair<KEY, KEY> k = std::move(mouse_keys_.front());
	mouse_keys_.pop();
	return k;
}

bool Mouse::HasMouseKey(){
	return !mouse_keys_.empty();
}

