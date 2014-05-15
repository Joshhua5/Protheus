#pragma once
#include <SDL.h>
#include "lua\lua.hpp"
namespace Pro{

	class Camera
	{
		// position and width
		SDL_Point position;
		SDL_Point dimensions;
	public:
		Camera();
		~Camera();
		SDL_Point* getPosition();
		void move(int x, int y);
		SDL_Point* getDimensions();
		void setPosition(int x, int y);
		void setDimensions(int w, int h);

		// Lua Functions

		static int lGetPosition(lua_State*);
		static int lSetPosition(lua_State*);
		static int lMove(lua_State*);
		static int lGetDimensions(lua_State*);
		static int lSetDimensions(lua_State*);
	};
}
