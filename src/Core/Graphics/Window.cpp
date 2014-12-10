#include "Window.h"

using namespace Pro;
using namespace Graphics;

static bool init = false;


void APIENTRY debug_callback(GLenum source​, GLenum type​, GLuint id​,
	GLenum severity​, GLsizei length​, const GLchar* message​, void* userParam​);

void Window::window_constructor(const WindowDefinition& def) {
	 
	if (init == false)
		glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, def.gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, def.gl_minor);
	glfwWindowHint(GLFW_RESIZABLE, def.resizable);
	glfwWindowHint(GLFW_VISIBLE, def.visible);
	glfwWindowHint(GLFW_DECORATED, def.decorated);
	glfwWindowHint(GLFW_STEREO, def.sterioscopic);
	glfwWindowHint(GLFW_RED_BITS, def.red_bits);
	glfwWindowHint(GLFW_GREEN_BITS, def.green_bits);
	glfwWindowHint(GLFW_BLUE_BITS, def.blue_bits);
	glfwWindowHint(GLFW_ALPHA_BITS, def.alpha_bits);
	glfwWindowHint(GLFW_DEPTH_BITS, def.depth_bits);
	glfwWindowHint(GLFW_STENCIL_BITS, def.stencil_bits);
	glfwWindowHint(GLFW_ACCUM_RED_BITS, def.accum_red_bits);
	glfwWindowHint(GLFW_ACCUM_GREEN_BITS, def.accum_green_bits);
	glfwWindowHint(GLFW_ACCUM_BLUE_BITS, def.accum_blue_bits);
	glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, def.accum_alpha_bits);
	glfwWindowHint(GLFW_AUX_BUFFERS, def.aux_buffers);
	glfwWindowHint(GLFW_SAMPLES, def.samples);
	glfwWindowHint(GLFW_REFRESH_RATE, def.refresh_rate);
	glfwWindowHint(GLFW_SRGB_CAPABLE, def.srgb_capable);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, def.debug);

	if (def.ogl_core_profile)
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(def.width, def.height, def.title.data(), NULL, NULL);
	
	setCurrent();

	if (init == false) {
		glewInit();
		init = true;
	}

	if (window == nullptr)
		return error.reportFatalNR("Unable to create window");

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);
 	glDebugMessageCallback((GLDEBUGPROC)&debug_callback, nullptr);

	glfwSetWindowUserPointer(window, new InputPointers);

	keyboard.attachWindow(window);


}

Window::Window(const WindowDefinition& def){
	window_constructor(def);
}

Window::Window(const string& title, const Vector2<int> dimensions){ 
	if (init == false)
		glfwInit();

	WindowDefinition def;
	def.title = title;
	def.width = dimensions.x;
	def.height = dimensions.y;

	window_constructor(def); 
}

Window::~Window(){
	delete glfwGetWindowUserPointer(window);
	if (window != nullptr)
		glfwDestroyWindow(window);
}

Keyboard& Window::getKeyboard() {
	return keyboard;
}

GLFWwindow* Window::getWindow() const {
	return window;
}

void Window::resize(const Vector2<int>& size) {
	glfwSetWindowSize(window, size.x, size.y);
}

void Window::rename(const string& title){
	glfwSetWindowTitle(window, title.data());
}

bool Window::isFocused() const {  
	return (glfwGetWindowAttrib(window, GLFW_FOCUSED) == GL_TRUE) ? true : false;
}

void Window::setCurrent() {
	glfwMakeContextCurrent(window);
}


void Window::startFrame() {
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT);
	glfwPollEvents();
}

void Window::endFrame() {
	glfwSwapBuffers(window);
}


void __stdcall debug_callback(GLenum source​, GLenum type​, GLuint id​,
	GLenum severity​, GLsizei length​, const GLchar* message​, void* userParam​) {
	error.reportFatal(string(message​, length​)); 
}