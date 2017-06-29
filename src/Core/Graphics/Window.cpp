#include "Window.h"

#ifdef __WINDOWS__
#include "Window_WIN.cpp"
#elif defined(__APPLE__)
#include "Window_OSX.h"
#elif defined(__LINUX__)
#error "Compile Test Output"
#include "Window_X11.cpp"
#endif

using namespace Pro;
using namespace Graphics;
using namespace std;

Window::Window(WindowDefinition& def)
{
    keyboard_changes_ = std::make_shared<Util::Pipe<std::pair<Input::KEY_PRESSED, Input::KEY>>>(128);
    mouse_changes_ = std::make_shared<Util::Pipe<Input::MouseEvent>>(256);
	WindowConstructor(def);
}

Window::Window(const std::string& title, const Vector2<int>& dimensions)
{
    keyboard_changes_ = std::make_shared<Util::Pipe<std::pair<Input::KEY_PRESSED, Input::KEY>>>(128);
    mouse_changes_ = std::make_shared<Util::Pipe<Input::MouseEvent>>(256);
    
	WindowDefinition def;
	def.title = title;
	def.width = dimensions.x;
	def.height = dimensions.y;

	WindowConstructor(def);
}

Input::Keyboard& Window::keyboard() {
	return keyboard_;
}

Input::Mouse& Window::mouse() {
	return mouse_;
}

bool Window::IsExitRequested() const {
	return is_exit_requested_;
}

unsigned Window::width() const {
	return dimension_.x;
}
unsigned Window::height() const {
	return dimension_.y;
}

Vector2<unsigned> Window::dimensions() const {
	return dimension_;
}

void Window::Close() {
	this->~Window();
}

float Window::Aspect() const {
	return  (float)dimension_.x / (float)dimension_.y;
}

void Window::PrintDetails() const {
	global_log.Report<LogCode::INFO>(std::string("OpenGL Window Details") +
		std::string("( Vendor: ") + std::string((const char*)glGetString(GL_VENDOR)) +
		std::string(", Renderer: ") + std::string((const char*)glGetString(GL_RENDERER)) +
		std::string(", Version: ") + std::string((const char*)glGetString(GL_VERSION)) +
		std::string(", GLSL: ") + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)) +
                                     std::string(")"), __FUNCTION__, __LINE__);
    global_log.Report<LogCode::INFO>("Best SIMD Supported: " +
                                     SIMDTitles[(unsigned)GetRuntimeSupport()], __FUNCTION__, __LINE__);
}

const Input::Mouse& Window::mouse() const {
	return mouse_;
}
const Input::Keyboard& Window::keyboard() const {
	return keyboard_;
}
