#pragma once

#include "BitCompare.h"

enum struct CLASS_FLAG{
	ACTIVE_STATE =		(long) BIT::_1,
	AREA =				(long) BIT::_2,
	GUID =				(long) BIT::_3,
	SCRIPTABLE =		(long) BIT::_4,
	LUA_CALLBACK =		(long) BIT::_5,
	NAME =				(long) BIT::_6,
	POSITION =			(long) BIT::_7,
	ENTITY =			(long) BIT::_8,
	INVENTORY =			(long) BIT::_9,
	ITEM =				(long) BIT::_10,
	SPRITE_ENTITY =		(long) BIT::_11,
	ANIMATED_ENTITY =	(long) BIT::_12,
	AVATAR =			(long) BIT::_13,
	ANIMATED_SPRITE =	(long) BIT::_14,
	SPRITE =			(long) BIT::_15,
	CAMERA =			(long) BIT::_16,
	GUI_ENTITY =		(long) BIT::_17,
	GUI_BUTTON =		(long) BIT::_18,
};

inline CLASS_FLAG operator|(CLASS_FLAG a, CLASS_FLAG b){
	return static_cast<CLASS_FLAG>(static_cast<int>(a) | static_cast<int>(b));
}