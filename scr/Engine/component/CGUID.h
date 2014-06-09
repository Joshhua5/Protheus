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

#include "..\lua\lib\lua.hpp" 
#include "..\util\LuaUtils.h"
#include "..\GUIDLookup.h"
namespace Pro{
	namespace Component{
		typedef unsigned int uint32;
		class CGUID
		{
		protected:
			uint32 guid;
		public:
			CGUID(const std::string& name);
			CGUID();
			~CGUID();

			uint32 getGUID();

			static int lGetGUID(lua_State*);

			// returns the Metatable assosiated with this object
			static inline string lGetMetatable(){
				return "component_guid_metatable";
			}
		};
	}
}
