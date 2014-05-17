#pragma once
#include <SDL_rect.h>
#include "lua\lua.hpp"
#include "Position.h"
#include "CGUID.h"

namespace Pro{
	namespace GameObject{ 

		class Entity : 
			public Position,
			public CGUID
		{
		protected: 
			bool active;
		public:
			Entity(const std::string& name);
			Entity();
			~Entity();
			  
			void activate();
			void deactivate();
			bool isActive();
			  
			// LUA Functions
			  
			static int lActivate(lua_State*);
			static int lDeactivate(lua_State*);
			static int lIsActive(lua_State*);
		};
	}
}