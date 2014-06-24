/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Stores and allocates GUID's to names provided.

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J. 
*************************************************************************/

#pragma once
#include <unordered_map>
#include <stack>
#include <iostream>
#include "lua\lib\lua.hpp"

namespace Pro{  
	typedef unsigned int uint32;
	class GUIDLookup{
	private:
		static std::unordered_map<std::string, uint32>* getNameMapping();
	public:
		static uint32  newGUID(const std::string& name);
		static void releaseGUID(const std::string& name);
		static void releaseGUID(uint32);
		static uint32  getGUID(const std::string& name);
		static std::string* getName(uint32 _id);
		 
		static int lGetGUID(lua_State*);
		static int lGetName(lua_State*);
	};
}