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

#include <string>
#include <vector>
#include "..\lua\lib\lua.hpp"

using namespace std;

namespace Pro{
	namespace Component{
		class Name
		{
			string name;
		public:
			void setName(const string& _name);
			const string getName();

			// returns the Metatable assosiated with this object
			/*constexpr*/ static const char* lGetMetatable(){
				return "component_name_metatable";
			}

			template<typename T>
			inline static void lGetFunctions(vector<luaL_Reg>& fields){
			}
		};
	}
}
