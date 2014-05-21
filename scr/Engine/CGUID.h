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
#include "lua\lua.hpp"
#include "GUIDLookup.h"
#include "LuaUtils.h"

namespace Pro{
	namespace Component{
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
		};
	}
}
