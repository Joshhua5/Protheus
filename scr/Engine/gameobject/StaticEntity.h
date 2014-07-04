/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	 
-------------------------------------------------------------------------
History:
- 25:06:2014: Waring J.
*************************************************************************/
#pragma once

#include "..\Components.h"

namespace Pro{
	namespace GameObject{
		using namespace Component;
		class StaticEntity : 
			Position,
			Area,
			Textured,
			Animated,
			CGUID
		{
		public:
			StaticEntity();
			~StaticEntity();


			// LUA Functions

			// returns the Metatable's name assosiated with this object
			static inline string lGetMetatable(){
				return "gameobject_static_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){ 
				Position::lGetFunctions<T>(fields);
				Area::lGetFunctions<T>(fields);
				Textured::lGetFunctions<T>(fields);
				Animated::lGetFunctions<T>(fields);
				CGUID::lGetFunctions<T>(fields);
			}
		}; 
	}
}

