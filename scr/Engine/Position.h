/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.
*************************************************************************/

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
#include "Math.h"
#include "lua\lua.hpp"
namespace Pro{
	namespace Component{
		class Position
		{
		protected:
			Math::Vector2 position;
		public:
			Position();
			~Position();

			Math::Vector2 getPosition();
			Math::Vector2* pGetPosition();
			void setPosition(Math::Vector2&);

			// LUA Functions

			static int lGetPosition(lua_State*);
			static int lSetPosition(lua_State*);
		}; 
	}
}
