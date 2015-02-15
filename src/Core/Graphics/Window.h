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
#include <Log.h> 
#include "TextureUnit.h"
#include "extern\glew.h"
#include "extern\glfw3.h"
#include "WindowDefinition.h"
#include <Parallel.h>

// INCLUDE FROM INPUT
#include "..\Input\Keyboard.h"
#include "..\Input\Mouse.h"
#include <smart_ptr.h>

namespace Pro {
	namespace Graphics { 
		using namespace std;
		using namespace Math;
		using namespace Input;

		class Window
		{
			GLFWwindow* window_;
			string window_title_;
			Vector2<unsigned> dimension_;
			Keyboard keyboard_;
			Mouse mouse_;
			 
			void window_constructor(const WindowDefinition& def); 

		public:
			Window(const WindowDefinition&);
			Window(const string& title, const Vector2<int> dimensions);
			~Window();

			void Rename(const string& title);
			void Resize(const Vector2<int>& size);
			bool IsFocused() const;
			GLFWwindow* GetWindow() const;
			void SetCurrent();
			bool IsExitRequested() const;

			void StartFrame();
			void EndFrame();

			unsigned width() const;
			unsigned height() const;
			Vector2<unsigned> dimensions() const;

			Keyboard& keyboard();
			Mouse& mouse();

			const Mouse& mouse() const;
			const Keyboard& keyboard() const;

			float aspect() const;
		};
	}
}