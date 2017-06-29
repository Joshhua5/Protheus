#include <PreDefinition.h>
#include "Window.h"

using namespace std;
using namespace Pro;
using namespace Graphics;
using namespace Math;

#ifdef __LINUX__

void event_update_thread(Display* display,
   Pipe<pair<KEY_PRESSED, KEY>>* keyboard_changes_,
   Pipe<pair<KEY_PRESSED, KEY>>* mouse_changes_,
   std::atomic<bool> update_running){
  while(update_running.load()){
    XEvent event;
    XNextEvent(display, &event);
    switch(event.type){
      case KeyPress:
      case KeyRelease:
      {
        MouseEvent m_event;
        m_event.posititon = {};
        m_event.state = (event.type == KeyRelease) ?
          KEY_PRESSED::PRESSED : KEY_PRESSED::RELEASED;
        m_event.key = event.keycode;
        mouse_changes_->Push(m_event);
      }
      break;
      case ButtonPress:
      case ButtonRelease:
      {
        keyboard_changes_->Push({event.keycode, (event.type == ButtonPress) ?
            KEY_PRESSED::PRESSED : KEY_PRESSED::RELEASED});
      }
      break;
      default:
    }
  }
}


bool Window::WindowConstructor(WindowDefinition& definition){
  display = XOpenDisplay((char*)0);

  GLint att = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, NULL};

  // OpenGL

  root = DefaultRootWindow(display);
  int scrnum = DefaultScreen(display);
  vi = glXChooseVisual(display, scrnum, att);
  cmap = XCreateColorMap(display, root, vi->visual, AllocNone);
  swa.colormap = cmap;
  swa.event_mask = ExposureMask | KeyPressMask;

  win = XCreateWindow(display, root, 0, 0, definition.width, definition.height,
    0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);


  // Create GL4.0 context

  int glAttr[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
    GLX_MINOR_VERSION_ARB, 0,
    GLX_CONTEXT_FLAGS, ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    0
  }

  int elemc;
  GLXFBConfig* fbcfg = glXChooseFBConfig(display, scrnum, NULL, &elemc);
  glc = glXCreateContextAttribsARB(display, fbcfg[0] , NULL, true, glAttr);

  XMapWindow(display, window);
  XStoreName(display, window, definition.title.data());
  CurrentContext();

  PrintDetails();

  update_running_ = true;
  updating_thread_ = std::thread(event_update_thread, display,
    &keyboard_changes_, mouse_changes_);
}


Window::~Window(){
  update_running_ = false;
  update_thread_.join();

  if(glc != 0)
    glXDestroyContext();
  if(win != 0)
    XDestryoWindow(display, win);
  if(cmap != 0)
    XFreeColorMap(display, cmap);
  if(display != nullptr)
    XCloseDisplay(display);

  glc = 0;
  win = 0;
  display = nullptr;
}

void Window::Rename(const std::string& title){
  window_title_ = title;
  XStoreName(display, window, title.data());
}

void Window::Resize(const Vector2<unsigned>& dimensions){
  dimension_ = dimensions;
  XResizeWindow(display, window, dimensions.x, dimensions.y);
  glViewPort(0, 0, dimensions.x, dimensions.y);
}

void Window::IsFocused(){
  Window focused;
  int state;
  XGetInputFocus(display, &focused, &state);
  return focused == win;
}

void Window::Focus(){
  XRaiseWindow(display, win);
}

void Window::IsOpen() const{
  return win != 0;
}

void Window::CurrentContext() {
  glXMakeCurrent(window, win, glc);
}

void Window::StartFrame() {
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT);
}

void Window::EndFrame() {
  glXSwapBuffers(display, win);
}


#endif
