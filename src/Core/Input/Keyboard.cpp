#include "Keyboard.h"

using namespace Pro;
using namespace Input;

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	static_cast<InputPointers*>(glfwGetWindowUserPointer(window))
		->keyboard->push({ (KEY)mods, (KEY)key });
} 

void Keyboard::attachWindow(GLFWwindow* window) {
	glfwSetKeyCallback(window, &keyboard_callback);
	static_cast<InputPointers*>(glfwGetWindowUserPointer(window))->keyboard = &keyboard_key;
	this->window = window;
}


bool Keyboard::hasKey() {
	return !keyboard_key.empty();
}

std::pair<KEY, KEY> Keyboard::poolKey() {
	if (!hasKey())
		return { KEY::KEY_UNKNOWN, KEY::KEY_UNKNOWN };
	std::pair<KEY, KEY> k = std::move(keyboard_key.front());
	keyboard_key.pop();
	return k;
}

KEY_PRESSED Keyboard::isKeyDown(KEY key) {
	return (KEY_PRESSED)glfwGetKey(window, (int)key);
}