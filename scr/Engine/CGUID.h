#pragma once
#include "lua\lua.hpp"
#include "GUIDLookup.h"

namespace Pro{
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

