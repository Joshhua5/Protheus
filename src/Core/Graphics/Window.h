/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <string> 
#include <Vector2.h>
#include <Error.h>
#include "extern\glfw3.h"
#include "extern\glcorearb.h"

namespace Pro {
	namespace Graphics {
		struct WindowDefinition {
			unsigned width = 800, height = 600;
			unsigned char gl_major = 3, gl_minor = 1;
			string title = "untitled";
			bool resizable = true;
			bool visible = true;
			bool decorated = true;
			bool sterioscopic = false;
			unsigned red_bits = 8;
			unsigned blue_bits = 8;
			unsigned green_bits = 8;
			unsigned alpha_bits = 8;
			unsigned depth_bits = 24;
			unsigned stencil_bits = 8;
			unsigned accum_red_bits = 0;
			unsigned accum_blue_bits = 0;
			unsigned accum_green_bits = 0;
			unsigned accum_alpha_bits = 0;
			unsigned aux_buffers = 0;
			unsigned samples = 0;
			unsigned refresh_rate = 0;
			bool srgb_capable = false;
			bool debug = false;
			bool ogl_core_profile = false; 
		};

		using namespace Math;
		using namespace std;
		class Window
		{
			GLFWwindow* window;
			string windowTitle;
			Vector2<unsigned> dimension;

		public:
			Window(const WindowDefinition&);
			~Window();

			void rename(const string& title);
			void resize(const Vector2<int>& size);
			bool isFocused() const;
			GLFWwindow* getWindow() const;
			void setCurrent();
			 
		};
	}
}