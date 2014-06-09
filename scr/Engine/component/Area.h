/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.
*************************************************************************/

#pragma once
#include <string>
#include "Math.h"
#include "..\lua\lib\lua.hpp"
namespace Pro{
	namespace Component {
		class Area
		{
		protected:
			Math::Vector2 dimensions;
		public:
			Area();
			~Area();

			Math::Vector2 getDimensions();
			Math::Vector2* pGetDimensions();
			void setDimensions(Math::Vector2&);

			// LUA Functions

			static int lGetDimensions(lua_State*);
			static int lSetDimensions(lua_State*);

			// returns the Metatable assosiated with this object
			static inline string lGetMetatable(){
				return "component_area_metatable";
			}
		};
	}
}
