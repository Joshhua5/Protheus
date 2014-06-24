/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	A camera class, which holdes it's position and field of view(Area)
	Contained inside a CameraContainer, multiple camera's can be created,
	but only the active camera will be used.
-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once 

#include <string>
#include <vector>
#include "..\Math.h"
#include "..\lua\lib\lua.hpp"
#include "..\component\Position.h"
#include "..\component\Area.h"
#include "..\component\CGUID.h"

using namespace std;

namespace Pro{
	namespace GameObject{
		class Camera : 
			public Component::Position,
			public Component::Area,
			public Component::CGUID
		{  
		public:
			Camera(const std::string& name);
			Camera();
			~Camera(); 
			void move(Math::Vector2&); 
			  
			// Lua Functions
			 
			static int lMove(lua_State*); 

			// returns the Metatable's name assosiated with this object
			static string lGetMetatable(){
				return "scene_camera_metatable";
			}

			template<typename T> 
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				Position::lGetFunctions<T>(fields);
				Area::lGetFunctions<T>(fields);
				CGUID::lGetFunctions<T>(fields);

				fields.push_back({ "move", (lua_CFunction)&T::lMove });
			}
		};
	}
}