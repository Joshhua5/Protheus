/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.
*************************************************************************/

#pragma once
#include "Math.h"
#include "lua\lua.hpp"

using namespace Pro;
class Volume
{
protected:
	Math::Vector2 dimensions;
public:
	Volume();
	~Volume();

	Math::Vector2 getDimensions();
	Math::Vector2* pGetDimensions();
	void setDimensions(Math::Vector2);

	// LUA Functions

	static int lGetDimensions(lua_State*);
	static int lSetDimensions(lua_State*);
};