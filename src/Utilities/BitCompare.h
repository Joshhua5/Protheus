/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:
A enum containing a number with a single bit checked
Allows for comparison with other types using the & to check
for set bits
-------------------------------------------------------------------------
History:
- 06:06:2014: Waring J.
*************************************************************************/

#pragma once

namespace Pro {
	namespace Util {

		//! Allow to check if a single bit has been checked 
		enum struct Bit {
			_0 = (int)0x0,
			_1 = (int)0x1,
			_2 = (int)0x2,
			_3 = (int)0x4,
			_4 = (int)0x8,
			_5 = (int)0x10,
			_6 = (int)0x20,
			_7 = (int)0x40,
			_8 = (int)0x80,

			_9 = (int)0x100,
			_10 = (int)0x200,
			_11 = (int)0x400,
			_12 = (int)0x800,
			_13 = (int)0x1000,
			_14 = (int)0x2000,
			_15 = (int)0x4000,
			_16 = (int)0x8000,

			_17 = (int)0x10000,
			_18 = (int)0x20000,
			_19 = (int)0x40000,
			_20 = (int)0x80000,
			_21 = (int)0x100000,
			_22 = (int)0x200000,
			_23 = (int)0x400000,
			_24 = (int)0x800000,

			_25 = (int)0x1000000,
			_26 = (int)0x2000000,
			_27 = (int)0x4000000,
			_28 = (int)0x8000000,
			_29 = (int)0x10000000,
			_30 = (int)0x20000000,
			_31 = (int)0x40000000, 
		};

		inline Bit operator|(Bit a, Bit b) {
			return static_cast<Bit>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
		}

		inline Bit operator&(Bit a, Bit b) {
			return static_cast<Bit>(static_cast<unsigned>(a) & static_cast<unsigned>(b));
		}

		/*! Check if the bit is set */
		inline bool CheckBit(int value, unsigned bit) {
			return static_cast<bool>((value >> bit) & 1);
		}
	}
}