/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <PreDefinition.h>
#include <string>
#include "../Profiler/Profiler.h"
#include <Vector2.h>
#include <Log.h>
#include "TextureUnit.h"
#include "WindowDefinition.h"
#include "OpenGL.h"
#include <Parallel.h>
#include <SIMD.h>

// INCLUDE FROM INPUT
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"
#include "../Input/Keys.h"

#if defined(__WINDOWS__)
#include <Windows.h>
#elif defined(__APPLE__)
#include "Window_OSX.h"
#elif defined(__LINUX__)
#endif

namespace Pro {
	namespace Graphics {
		class Window
		{
#if defined(__WINDOWS__)
			// Windows Render Context
			HGLRC hRC = nullptr;
			// Windows Device Context
			HDC hDC = nullptr;
			// Window Window Handle
			HWND hWnd = nullptr;
			// ???
			HINSTANCE hInstance = nullptr;

			// Private functions for WndProc
			int WndClose();
			int WndKeyDown(WPARAM);
			int WndKeyUp(WPARAM);
			int WndResize(LPARAM);

			#elif defined(__APPLE__)
			CocoaWindow window;
			void WindowUpdate();
			std::chrono::milliseconds update_period;
			std::atomic<bool> updating;
			std::thread updating_thread;
			//! mouse_id, 0:left, 1:right 2:other
		public:
			void RegisterMouseEvent(int mouse_id, int x, int y, bool wasReleased);
			void RegisterKeyboardEvent(int key_code, bool wasReleased);
		private:

#elif defined(__LINUX__)
			Display* display;
			XID root;
			XID window;
			XVisualInfo* vi;
			Colormap cmap;
			XSetWindowAttributes swa;
			GLXContext glc;
			XWindowAttributes gwa;
			XEvent xev;

			std::thread updating_thread_;
			std::atomic<bool> update_running_;
#endif
			bool is_exit_requested_ = false;

			//! Input has reference to these Mouse::AttachWindow Keyboard::AttachWindow
			bool keys_[(unsigned) Input::KEY::SIZE];
			Math::Vector2<int> mouse_position;
            std::shared_ptr<Util::Pipe<std::pair<Input::KEY_PRESSED, Input::KEY>>> keyboard_changes_;
            std::shared_ptr<Util::Pipe<Input::MouseEvent>> mouse_changes_;
			bool init_ = false;

			// A copy of the definition used to construct the window
			// and which version of OpenGL which was returned
			WindowDefinition WindowDef;

			std::string window_title_;
			Math::Vector2<unsigned> dimension_;
			Input::Keyboard keyboard_;
			Input::Mouse mouse_;

			bool WindowConstructor(WindowDefinition& def);

		public:
			Window(WindowDefinition&);
			Window(const std::string& title, const Math::Vector2<int>& dimensions);
			~Window();

			//! Changes the titles of this window
			void Rename(const std::string& title);
			//! Resized this window with &dimensions
			void Resize(const Math::Vector2<unsigned>& dimensions);
			//! Checks if the use has this window focused
			bool IsFocused() const;
			//! Gives this window focus
			void Focus();
			//! Returns true if the window has a close request
			bool IsExitRequested() const;
			//! Checks if a window is still open and/or created successfully
			bool IsOpen() const;
			//! Closes the window
			void Close();


			//! Sets the current OGL context
			void CurrentContext();
			//! Clears the frame ready for the next
			void StartFrame();
			//! Flushes OpenGL and presents the frame
			void EndFrame();

			void PrintDetails() const;

			//! Polls the window for new events
			//! Must be called from the main thread
			void Update();
			//! Returns the width of the window
			unsigned width() const;
			//! Returns the height of the window
			unsigned height() const;
			//! Returns the width and height of the window
			Math::Vector2<unsigned> dimensions() const;

			//! Returns reference to the Pro::Input::Keyboard input
			//! Each input is specific to this window
			Input::Keyboard& keyboard();
			//! Returns reference to the Pro::Input::Mouse input
			//! Each input is specific to this window
			Input::Mouse& mouse();

			//! Returns constant reference to the Pro::Input::Mouse input
			//! Each input is specific to this window
			const Input::Mouse& mouse() const;
			//! Returns constant reference to the Pro::Input::Keyboard input
			//! Each input is specific to this window
			const Input::Keyboard& keyboard() const;

			//! Returns the aspect ratio of the window
			float Aspect() const;
		};
	}
}

#ifdef __APPLE__
//! Create a Cocoa Window and returns a pointer to the window
struct CocoaWindow CreateCocoaWindow(Pro::Graphics::Window* window, int width, int height, int x, int y,const char* title, unsigned char title_length, bool fullscreen);
//! Resizes a window, returns false is the window isn't resizable
bool ResizeCocoaWindow(struct CocoaWindow* window, int width, int height);
//! Sets the background colour of the window
void SetBackgroundColour(struct CocoaWindow* window, unsigned char r,unsigned char g,unsigned char b, unsigned char a);
//! Ensures that this window is set as the current context
void MakeCurrentContext(struct CocoaWindow* window);
//! Swaps the frame buffer
void SwapBuffer(struct CocoaWindow* window);

void ReleaseCocoaWindow(struct CocoaWindow* window);

void CocoaSetCurrent(struct CocoaWindow* window);

void CocoaUpdateWindow(struct CocoaWindow* window);

void CocoaRenameWindow(struct CocoaWindow* window, const char* string);
#endif
