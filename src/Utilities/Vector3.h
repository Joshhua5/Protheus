/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.

*************************************************************************/
 
#ifdef __SSE
#include <xmmintrin.h>
#endif

#pragma once
namespace Pro{
	namespace Math{
		template<typename T>
		struct Vector3{
			T x, y, z;

			Vector3() {

			}
			 
		};
	}
}
