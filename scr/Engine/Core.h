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
#include "Scene.h"
#include "Renderer.h"
#include "Timer.h" 
#include "Network.h"
#include "GUIDLookup.h"
#include "Math.h"
#include "Window.h"
#include "SpriteManager.h" 
#include "EventHandeler.h"

namespace Pro{ 
	class Core
	{ 
		bool running = false;

		bool update();
		bool render();
	public: 
		// Core functions 
		Core::Core();
		  
		bool loadSpriteSheet(const std::string &path, const std::string &data);
		bool loadSpriteAnimations(const std::string& path);
		bool loadScene(const std::string &path, const std::string& data);

		// x : position X, y : position Y
		// w : width, h : height
		void setWindowDimensions(SDL_Rect rect);
		// Sets the name of the window
		void setWindowTitle(const std::string& name);
		/* Runs the loop once and stops,
		   for user modification of the map */
		void run();

		// Change window details before Starting the engine
		bool start();
		bool cleanup();  
		  
		// Event Functions

		std::vector<SDL_Event>* pollKeyEvents();
		std::vector<SDL_Event>* pollMouseEvents();
		std::vector<SDL_Event>* pollSystemEvents();
		std::vector<SDL_Event>* pollUserEvents();

		// Map Functions
		 
		static int lUpdate(lua_State*);
	}; 
}