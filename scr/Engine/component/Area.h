/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Used to provideo an object with volume
-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.
*************************************************************************/

#pragma once

#include <string>
#include <vector>
#include "..\Math.h"
#include "..\util\LuaUtils.h"
using namespace std;

namespace Pro{
	namespace Component {
		using namespace Math;

		class Area
		{
		protected:
			Vector2 dimensions;
		public:
			Area(const Vector2& dimensions);  
			Area() {}

			Vector2 getDimensions() const;

			// returns a pointer to the internal dimension
			Vector2* pGetDimensions();

			// the width and height multiplied together
			float getVolume() const;

			void setDimensions(const Vector2&);
			void setDimensions(const Vector2&&);

			// LUA Functions

			static int lGetDimensions(lua_State*);
			static int lSetDimensions(lua_State*);

			// returns the Metatable assosiated with this object
			static inline string lGetMetatable(){
				return "component_area_metatable";
			}
			template<typename T>
			static void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "getDimensions", (lua_CFunction)&T::lGetDimensions });
				fields.push_back({ "setDimensions", (lua_CFunction)&T::lSetDimensions });
			}
		};
	}
}
