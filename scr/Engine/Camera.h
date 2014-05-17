#pragma once
#include <SDL_Rect.h>

#include "CGUID.h"
#include "Math.h"
#include "lua\lua.hpp"
#include "Position.h"
#include "Volume.h"

namespace Pro{
	namespace Scene{
		class Camera : 
			public Position,
			public Volume, 
			public CGUID
		{  
		public:
			Camera(const std::string& name);
			Camera();
			~Camera(); 
			void move(Math::Vector2); 
			  
			// Lua Functions
			 
			static int lMove(lua_State*); 
		};
	}
}