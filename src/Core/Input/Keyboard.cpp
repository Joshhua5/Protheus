#include "Keyboard.h"

using namespace Pro;
using namespace Input;

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	static_cast<InputPointers*>(glfwGetWindowUserPointer(window))
		->keyboard->push({ (KEY)mods, (KEY)key });
} 

void Keyboard::AttachWindow(GLFWwindow* window) {
	glfwSetKeyCallback(window, &keyboard_callback);
	static_cast<InputPointers*>(glfwGetWindowUserPointer(window))->keyboard = &keyboard_key_;
	this->window_ = window;
}


bool Keyboard::hasKey() {
	return !keyboard_key_.empty();
}

std::pair<KEY, KEY> Keyboard::PollKey() {
	if (!hasKey())
		return { KEY::KEY_UNKNOWN, KEY::KEY_UNKNOWN };
	std::pair<KEY, KEY> k = std::move(keyboard_key_.front());
	keyboard_key_.pop();
	return k;
}

KEY_PRESSED Keyboard::IsKeyDown(KEY key) {
	return (KEY_PRESSED)glfwGetKey(window_, (int)key);
}