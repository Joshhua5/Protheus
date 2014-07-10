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
#include "..\util\ClassDefinition.h"
#include "Entity.h"

namespace Pro{
	namespace GameObject{
		using namespace Component;
		class DynamicEntity :
			public Position,
			public Area,
			public ActiveState,
			public CScriptable,
			public Textured,
			public Animated,

			public Entity
		{
		public:
			DynamicEntity(const string& name);
			DynamicEntity();
			// LUA Functions

			// returns the Metatable's name assosiated with this object
			static inline string lGetMetatable(){
				return "gameobject_dynamic_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				Position::lGetFunctions<T>(fields);
				Area::lGetFunctions<T>(fields);
				ActiveState::lGetFunctions<T>(fields);
				CScriptable::lGetFunctions<T>(fields);
				Textured::lGetFunctions<T>(fields);
				Animated::lGetFunctions<T>(fields);
				CGUID::lGetFunctions<T>(fields);
			}

			static const ClassDefinition getDefinition();
		};
	}
}
