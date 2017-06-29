#include "Window.h"

#ifdef __WINDOWS__

using namespace Pro;
using namespace Graphics;
using namespace Input;

// Static member functions to private functions
// Used so WndProc can manipulate data inside of it's Window 

void __stdcall ogl_debug_callback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar* message, void* userParam) { 
	global_log.Report<LogCode::FATAL>(
		"OGL Context: " + std::string(message, length) + 
		"\nSource: " + std::to_string(source) + 
		"\nType: " + std::to_string(type) + 
		"\nID: " + std::to_string(id) + 
		"\nSeverity: " + std::to_string(severity)
		, __FUNCTION__, __LINE__);
}

static int(Window::*ptrWndResize)(LPARAM);
static int(Window::*ptrWndClose)(void);
static int(Window::*ptrWndKeyUp)(WPARAM);
static int(Window::*ptrWndKeyDown)(WPARAM);
 
//! TODO Need a method to attach a user pointer to hWnd to pass into this function
int WINAPI WndProc(HWND	hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Window* window_ptr = (Window*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (window_ptr == nullptr)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	switch (uMsg) {
	case WM_CLOSE:
		return (window_ptr->*ptrWndClose)();
	case WM_KEYDOWN:
		return (window_ptr->*ptrWndKeyDown)(wParam);
	case WM_KEYUP:
		return (window_ptr->*ptrWndKeyUp)(wParam);
	case WM_SIZE:
		return (window_ptr->*ptrWndResize)(lParam);
	}
	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int Window::WndKeyDown(WPARAM wParam) {
	if (keyboard_changes_ != nullptr)
		keyboard_changes_->Push({ KEY_PRESSED::PRESSED, (KEY)wParam });
	keys_[(int)wParam] = TRUE;
	return 0;
}

int Window::WndKeyUp(WPARAM wParam) {
	if (keyboard_changes_ != nullptr)
		keyboard_changes_->Push({ KEY_PRESSED::RELEASED, (KEY)wParam });
	keys_[(int)wParam] = FALSE;
	return 0;
}

int Window::WndClose() {
	is_exit_requested_ = true;
	PostQuitMessage(0);
	return 0;
}

int Window::WndResize(LPARAM lParam) {  
	Resize({LOWORD(lParam),  HIWORD(lParam)});
	return 0;
}


Window::~Window() {
	is_exit_requested_ = true;

	if (WindowDef.fullscreen) {
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	//Release Render Context
	if (hRC) {
		if (!wglMakeCurrent(NULL, NULL))
			global_log.Report<LogCode::FATAL>("Unable to release Device Context and Render Context", __FUNCTION__, __LINE__);
		if (!wglDeleteContext(hRC))
			global_log.Report<LogCode::FATAL>("Unable to delete Render Context", __FUNCTION__, __LINE__);
	}
	// Release Device Context
	if (hDC && !ReleaseDC(hWnd, hDC))
		global_log.Report<LogCode::FATAL>("Unable to release device context", __FUNCTION__, __LINE__);

	if (hWnd && !DestroyWindow(hWnd))
		global_log.Report<LogCode::FATAL>("Unable to destroy window", __FUNCTION__, __LINE__);

	if (!UnregisterClass("Protheus", hInstance))
		global_log.Report<LogCode::FATAL>("Unable to unregister class", __FUNCTION__, __LINE__);

	hRC = 0;
	hDC = 0;
	hWnd = 0;
	hInstance = 0;
}



bool Window::WindowConstructor(WindowDefinition& def) {

	memset(&keys_, (unsigned)KEY_PRESSED::RELEASED, sizeof(bool)* (unsigned)KEY::SIZE);

	GLuint PixelFormat;
	WNDCLASS wc;
	DWORD dwExStyle;
	DWORD dwStyle;
	RECT WindowRect;
	WindowRect.left = 0l;
	WindowRect.right = def.width;
	WindowRect.top = 0l;
	WindowRect.bottom = def.height;

	hInstance = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)&WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Protheus";

	if (!RegisterClass(&wc)) {
		global_log.Report<LogCode::FATAL>("Failed to register window class.", __FUNCTION__, __LINE__);
		return false;
	}

	if (def.fullscreen) {
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(DEVMODE));
		dmScreenSettings.dmSize = sizeof(DEVMODE);
		dmScreenSettings.dmPelsWidth = def.width;
		dmScreenSettings.dmPelsHeight = def.height;
		dmScreenSettings.dmBitsPerPel = def.depth_bits;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			global_log.Report<LogCode::FATAL>("Unable to launch in fullscreen, attempting to launch windowed", __FUNCTION__, __LINE__);
			def.fullscreen = false;
		}
	}

	// If fullscreen creation was successful
	dwExStyle = WS_EX_APPWINDOW | (def.fullscreen ? 0 : WS_EX_WINDOWEDGE);
	dwStyle = (def.fullscreen) ? WS_POPUP : WS_OVERLAPPEDWINDOW;

	if (def.custom_mouse && def.fullscreen)
		ShowCursor(FALSE);

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	if (!(hWnd = CreateWindowEx(dwExStyle,
		"Protheus",
		def.title.data(),
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top, NULL, NULL, hInstance, NULL))) {

		this->~Window();
		global_log.Report<LogCode::FATAL>("Error creating window", __FUNCTION__, __LINE__);
		return false;
	}

	// Prevent WndProc from calling functions before their set
	SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		def.red_bits + def.blue_bits + def.green_bits + def.alpha_bits,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,
		8,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	// Check things were created correctly
	if (!(hDC = GetDC(hWnd))) {
		this->~Window();
		global_log.Report<LogCode::FATAL>("Can't create a GL Device Context.", __FUNCTION__, __LINE__);
		return false;
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {
		this->~Window();
		global_log.Report<LogCode::FATAL>("Unable to find a suitable pixel format.", __FUNCTION__, __LINE__);
		return false;
	}
	if (!SetPixelFormat(hDC, PixelFormat, &pfd)) {
		this->~Window();
		global_log.Report<LogCode::FATAL>("Can't apply pixel format", __FUNCTION__, __LINE__);
		return false;
	}

	if (!(hRC = wglCreateContext(hDC))) {
		this->~Window();
		global_log.Report<LogCode::FATAL>("Can't create a GL Rendering Context", __FUNCTION__, __LINE__);
		return false;
	}

	if (!wglMakeCurrent(hDC, hRC)) {
		this->~Window();
		global_log.Report<LogCode::FATAL>("Can't activate GL Rendering Context", __FUNCTION__, __LINE__);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	// Set user pointer to this window
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)this);
	// Set function pointers for WndProc
	ptrWndResize = &Window::WndResize;
	ptrWndClose = &Window::WndClose;
	ptrWndKeyDown = &Window::WndKeyDown;
	ptrWndKeyUp = &Window::WndKeyUp;

	// Set user pointer to this window
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)this);

	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	if (init_ == false) {
		glewExperimental = GL_TRUE;
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			global_log.Report<LogCode::FATAL>("Error before GlewInit", __FUNCTION__, __LINE__);
		err = glewInit();
		if (err != GLEW_OK)
			global_log.Report<LogCode::FATAL>("Error inside GlewInit", __FUNCTION__, __LINE__);
		else {
			init_ = true;
			global_log.Report<LogCode::MESSAGE>("Successfully initialised GLEW", __FUNCTION__, __LINE__);
		}
	}
	else
		global_log.Report<LogCode::MESSAGE>("GLEW has already initialised", __FUNCTION__, __LINE__);

	// Initialise final context
	if (glewIsSupported("WGL_ARB_create_context") | glewIsSupported("WGL_ARB_create_context_profile")) {
		int attribList[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, def.gl_major,
			WGL_CONTEXT_MINOR_VERSION_ARB, def.gl_minor,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifdef _DEBUG
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
			0
		};

		HGLRC hGLRC = 0;

		if (!(hGLRC = wglCreateContextAttribsARB(hDC, 0, attribList))) {
			// context creation failed, falling back onto 3.3
			global_log.Report<LogCode::FAULT>("Unable to create requestest context, falling back to 3.3 : " + glGetErrorString(), __FUNCTION__, __LINE__);
			attribList[1] = 3;
			attribList[3] = 3;
			if (!(hGLRC = wglCreateContextAttribsARB(hDC, 0, attribList))) {
				global_log.Report<LogCode::FATAL>("Unable to create fallback context 3.3 : " + glGetErrorString(), __FUNCTION__, __LINE__);
				this->~Window();
				return false;
			}
			if (!wglMakeCurrent(hDC, hGLRC))
				global_log.Report<LogCode::FATAL>("Unable to bind final context", __FUNCTION__, __LINE__);
			else {
				// clean up old context
				wglDeleteContext(hRC);
				hRC = hGLRC;
			}
		}
	}
	else
		global_log.Report<LogCode::FATAL>("WGL_ARB_create_context is unsupported, try updating graphics drivers", __FUNCTION__, __LINE__);



	dimension_ = Vector2<unsigned>(def.width, def.height);
	// Initialize the texture unit class
	TextureUnit();

	GLint err = glGetError();
	if (err != GL_NO_ERROR)
		global_log.Report<LogCode::FATAL>("Error Initializing TextureUnit, did GLEW finish successfully?", __FUNCTION__, __LINE__);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);


	keyboard_.AttachWindow(keyboard_changes_, keys_);
	mouse_.AttachWindow(mouse_changes_, &mouse_position);
	// Poll for context information
	glGetIntegerv(GL_MAJOR_VERSION, &def.gl_major);
	glGetIntegerv(GL_MINOR_VERSION, &def.gl_minor);

	PrintDetails();

	if (def.debug && def.gl_major >= 4 && def.gl_minor >= 3) { 
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback((GLDEBUGPROC)&ogl_debug_callback, nullptr);
	}

	err = glGetError();
	if (err != GL_NO_ERROR)
		global_log.Report<LogCode::FATAL>("Unable to create window, " + glGetErrorString(err) + " " + std::string((char*)glewGetErrorString(err)), __FUNCTION__, __LINE__);

	WindowDef = def;

	return true;
}

void Window::Resize(const Vector2<unsigned>& size) { 
	glViewport(0, 0, size.x, size.y);
	dimension_ = size;
}

void Window::Rename(const std::string& title) {
	SetWindowText(hWnd, title.data());
}

bool Window::IsFocused() const {
	return GetFocus() == hWnd;
}

bool Window::IsOpen() const {
	return (hWnd == nullptr) ? false : true;
}

void Window::Focus() {
	SetFocus(hWnd);
}

void Window::StartFrame() {
	MSG msg;
	while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT);

}


void Window::Update() {}

void Window::EndFrame() {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		global_log.Report<LogCode::WARNING>("Uncaught OpenGL error from last frame" + glGetErrorString(error), __FUNCTION__, __LINE__);
	}
	SwapBuffers(hDC);
}

#endif
