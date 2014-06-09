/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	A enum containing a number with a sinle bit checked
	Allows for comparison with other types using the & to check
	for set bits
-------------------------------------------------------------------------
History:
- 06:06:2014: Waring J.
*************************************************************************/

#pragma once


enum struct BIT{
	_1 = (long) 0x1,
	_2 = (long) 0x2,
	_3 = (long) 0x4,
	_4 = (long) 0x8,
	_5 = (long) 0x10,
	_6 = (long) 0x20,
	_7 = (long) 0x40,
	_8 = (long) 0x80,

	_9 = (long) 0x100,
	_10 = (long) 0x200,
	_11 = (long) 0x400,
	_12 = (long) 0x800,
	_13 = (long) 0x1000,
	_14 = (long) 0x2000,
	_15 = (long) 0x4000,
	_16 = (long) 0x8000,

	_17 = (long) 0x10000,
	_18 = (long) 0x20000,
	_19 = (long) 0x40000,
	_20 = (long) 0x80000,
	_21 = (long) 0x100000,
	_22 = (long) 0x200000,
	_23 = (long) 0x400000,
	_24 = (long) 0x800000,

	_25 = (long) 0x1000000,
	_26 = (long) 0x2000000,
	_27 = (long) 0x4000000,
	_28 = (long) 0x8000000,
	_29 = (long) 0x10000000,
	_30 = (long) 0x20000000,
	_31 = (long) 0x40000000,
	_32 = (long) 0x80000000, 
};

inline BIT operator|(BIT a, BIT b){
	return static_cast<BIT>(static_cast<long>(a) | static_cast<long>(b));
} 

inline BIT operator&(BIT a, BIT b){
	return static_cast<BIT>(static_cast<long>(a) & static_cast<long>(b));
}

inline bool checkBit(int value, int bit){
	return static_cast<bool>((value >> bit) & 1) ;
}