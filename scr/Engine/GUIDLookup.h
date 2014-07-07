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

using namespace std;

namespace Pro{  
	typedef unsigned long game_id;

	class GUIDLookup{
	private:
		static unordered_map<string, game_id>* getNameMapping();
	public:
		static game_id  newGUID(const string& name);
		static void releaseGUID(const string& name);
		static void releaseGUID(const game_id);
		static game_id  getGUID(const string& name);
		static string* getName(const game_id _id);
		 
		static int lGetGUID(lua_State*);
		static int lGetName(lua_State*);
	};
}