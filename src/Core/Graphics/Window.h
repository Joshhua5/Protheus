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
#include "extern\glew.h"
#include "extern\glfw3.h"
#include "WindowDefinition.h"

// INCLUDE FROM INPUT
#include "..\Input\Keyboard.h"

namespace Pro {
	namespace Graphics { 
		using namespace std;


		class Window
		{
			GLFWwindow* window;
			string windowTitle;
			Vector2<unsigned> dimension;
			Keyboard keyboard;
			 
			void window_constructor(const WindowDefinition& def);

		public:
			Window(const WindowDefinition&);
			Window(const string& title, const Vector2<int> dimensions);
			~Window();

			void rename(const string& title);
			void resize(const Vector2<int>& size);
			bool isFocused() const;
			GLFWwindow* getWindow() const;
			void setCurrent();

			void startFrame();
			void endFrame();

			Keyboard& getKeyboard();
		};
	}
}