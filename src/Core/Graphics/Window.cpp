#include "Window.h"

using namespace Pro;
using namespace Graphics;

static bool init = false;


void APIENTRY debug_callback(GLenum source​, GLenum type​, GLuint id​,
	GLenum severity​, GLsizei length​, const GLchar* message​, void* userParam​);

void glfw_error_callback(int error_code, const char* desctiption) {
	Pro::global_log.Report<LogCode::FATAL>(desctiption, "Window CALLBACK (Line is error code)", error_code);
}

void Window::window_constructor(const WindowDefinition& def) {
	 
	if (init == false)
		glfwInit();

	glewExperimental = true;
	glfwSetErrorCallback(&glfw_error_callback);

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

	dimension_ = { def.width, def.height };
	window_ = glfwCreateWindow(def.width, def.height, def.title.data(), NULL, NULL);
	
	SetCurrent();

	if (init == false) {
		glewInit();
		init = true;
	}

	if (window_ == nullptr) {
	Pro::global_log.Report<LogCode::FATAL>("Unable to create window", __FUNCTION__, __LINE__);
	return;
	}
	// Initialize the texture unit class
	TextureUnit();

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
 	glDebugMessageCallback((GLDEBUGPROC)&debug_callback, nullptr);

	glfwSetWindowUserPointer(window_, new InputPointers);

	keyboard_.AttachWindow(window_);
	mouse_.AttachWindow(window_); 

	GLuint err = glGetError();
	if (err != GL_NO_ERROR)
		global_log.Report<LogCode::FATAL>("Unable to create window, " + std::string((char*)glewGetErrorString(err)), __FUNCTION__, __LINE__);
	 
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
	delete glfwGetWindowUserPointer(window_);
	if (window_ != nullptr)
		glfwDestroyWindow(window_);
}

Keyboard& Window::keyboard() {
	return keyboard_;
}

Mouse& Window::mouse() {
	return mouse_;
}

GLFWwindow* Window::GetWindow() const {
	return window_;
}

void Window::Resize(const Vector2<int>& size) {
	glfwSetWindowSize(window_, size.x, size.y);
}

void Window::Rename(const string& title){
	glfwSetWindowTitle(window_, title.data());
}

bool Window::IsFocused() const {  
	return glfwGetWindowAttrib(window_, GLFW_FOCUSED) != 0 ? true : false;
}

void Window::SetCurrent() {
	glfwMakeContextCurrent(window_);
}

bool Window::IsExitRequested() const {
	return glfwWindowShouldClose(window_) != 0;
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

void Window::StartFrame() {
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT);
	glfwPollEvents();
}

void Window::EndFrame() {
	glfwSwapBuffers(window_);
}


float Window::aspect() const {
	return  (float)dimension_.x / (float)dimension_.y ;
}

void __stdcall debug_callback(GLenum source​, GLenum type​, GLuint id​,
	GLenum severity​, GLsizei length​, const GLchar* message​, void* userParam​) {
	Pro::global_log.Report<LogCode::FATAL>(string(message​, length​), __FUNCTION__, __LINE__); 
}


const Mouse& Window::mouse() const {
	return mouse_;
}
const Keyboard& Window::keyboard() const {
	return keyboard_;
}