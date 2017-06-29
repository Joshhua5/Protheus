#pragma once
/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 22/02/2016 Waring J.

*************************************************************************/

#include <Vector4.h>

namespace Pro {
	namespace Graphics {
		typedef Pro::Math::Vector4<char> Color;

		namespace COLORS {
			const Color BLACK = Color({ (char)0, (char)0, (char)0, (char)0 });
			const Color WHITE = Color({ (char)255, (char)255, (char)255, (char)0 });
			const Color RED = Color({ (char)255, (char)0, (char)0, (char)0 });
		}
	}
}