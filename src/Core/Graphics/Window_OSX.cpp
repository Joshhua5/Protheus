#include "Window.h"
#include "Window_OSX.h"

using namespace Pro;
using namespace Graphics;
using namespace Input;

#ifdef __APPLE__

void Window::WindowUpdate(){
    while(updating){
        std::this_thread::sleep_for(update_period);
    }
}

bool Window::WindowConstructor(WindowDefinition& def) {
    WindowDef = def;

    // Set all key posititons to released
    memset(&keys_[0], (unsigned)KEY_PRESSED::RELEASED, sizeof(bool) * (unsigned)KEY::SIZE);

    keyboard_.AttachWindow(keyboard_changes_, keys_);
    mouse_.AttachWindow(mouse_changes_, &mouse_position);

    window = CreateCocoaWindow(this, def.width, def.height, 0, 0, def.title.data(), def.title.length(), def.fullscreen);

    Focus();
    // Check that the OGL Context and Window were created successfully
    if(window.window == nullptr)
        global_log.Report<LogCode::FATAL>("Failed to create a window", __FUNCTION__, __LINE__);
    if(window.glContext == nullptr)
        global_log.Report<LogCode::FATAL>("Failed to grab a OpenGL Context", __FUNCTION__, __LINE__);

    CocoaSetCurrent(&window);
    MakeCurrentContext(&window);

    GL_CONTEXT_CREATED = true;

    dimension_ = Vector2<unsigned>(def.width, def.height);

    // Initialize the texture unit class
    TextureUnit();

    updating = true;
    update_period = std::chrono::milliseconds(10);

    updating_thread = std::thread(&Window::WindowUpdate, this);

    //glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    //if (def.debug && def.gl_major >= 4 && def.gl_minor >= 3)
       // glDebugMessageCallback((GLDEBUGPROC) &ogl_debug_callback, nullptr);

    // Poll for context information
    PrintDetails();

    GLuint err = glGetError();
    if (err != GL_NO_ERROR)
        global_log.Report<LogCode::FATAL>("Unable to create window, OGL error Code:" + std::to_string(err), __FUNCTION__, __LINE__);
    return true;
}

Window::~Window(){
    ReleaseCocoaWindow(&window);
    GL_CONTEXT_CREATED = false;
    global_profiler.DisableGPUProfile();
}

bool Window::IsOpen() const{
    return window.window != nullptr;
}

void Window::StartFrame() {
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT);
}

void Window::Update(){
    CocoaUpdateWindow(&window);
}

void Window::EndFrame(){
    SwapBuffer(&window);
    auto error = glGetError();
    if(error != GL_NO_ERROR)
        global_log.Report<LogCode::WARNING>("OpenGL Error caught at end of frame: " + glGetErrorString(error), __FUNCTION__, __LINE__);
}

void Window::Focus(){
    CocoaSetCurrent(&window);
}

void Window::CurrentContext(){
    MakeCurrentContext(&window);
}

void Window::RegisterMouseEvent(int mouse_id, int x, int y, bool wasReleased){
    mouse_position.x = x;
    mouse_position.y = y;
    MouseEvent event; 
    event.position.x = x;
    event.position.y = y;
    event.state =(wasReleased ? KEY_PRESSED::PRESSED : KEY_PRESSED::RELEASED);
    event.key = (KEY)mouse_id;
    mouse_changes_->Push(event);
}

void Window::RegisterKeyboardEvent(int key, bool wasReleased){
    keys_[key] = wasReleased;
    keyboard_changes_->Push(std::make_pair((wasReleased ? KEY_PRESSED::PRESSED : KEY_PRESSED::RELEASED), (KEY)key));
}

void Window::Rename(const std::string &title){
    CocoaRenameWindow(&window, title.data());
}

void Window::Resize(const Vector2<unsigned> &dimensions){
    dimension_ = dimensions;
    ResizeCocoaWindow(&window, dimensions.x, dimensions.y);
    glViewport(0, 0, dimensions.x, dimensions.y);
}

#endif
