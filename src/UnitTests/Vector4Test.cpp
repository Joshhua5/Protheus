#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Pro;
using namespace Math;

int main() {
	char exit_status = 0;
	{

		Vector4<int> x(1);
		Vector4<int> y(1, 2, 3, 4);
		Vector4<int> z({ 4, 8, 2, 2 });
		Vector4<int> w(y);
		Vector4<int> r(Vector4<int>(2));

		if (x.x != 1 || x.y != 1)
			exit_status |= 1;
		if (y.x != 1 || y.y != 2)
			exit_status |= 1;
		if (z.x != 4 || z.y != 8)
			exit_status |= 1;
		if (w.x != 1 || w.y != 2)
			exit_status |= 1;
		if (r.x != 2 || r.y != 2)
			exit_status |= 1;

		// Addition

		auto result = x + y;
		if (result.x != 2 || result.y != 3)
			exit_status |= 2;

		result += x;

		if (result.x != 3 || result.y != 4)
			exit_status |= 2;

		// Subtraction

		result = y - x;
		if (result.x != 0 || result.y != 1)
			exit_status |= 4;

		result -= x;

		if (result.x != -1 || result.y != 0)
			exit_status |= 4;

		// Division

		result = z / r;
		if (result.x != 2 || result.y != 4)
			exit_status |= 8;

		result /= r;

		if (result.x != 1 || result.y != 2)
			exit_status |= 8;

		// Multi

		result = z * r;
		if (result.x != 8 || result.y != 16)
			exit_status |= 16;

		result *= r;

		if (result.x != 16 || result.y != 32)
			exit_status |= 16;

		// Cast 

		auto cast = x.Cast<float>();
		if (cast.x != 1.f || cast.y != 1.f)
			exit_status |= 32;

		// Equals

		if (x != x)
			exit_status |= 64;
		if (!x.Equals(1, 1, 1, 1))
			exit_status |= 64;
		if (!(x == x))
			exit_status |= 64;


		// Contains

		if (!z.Contains({ 5, 9 }))
			exit_status |= 128;

		if (z.Contains({ 3, 9 }))
			exit_status |= 128;

		if (z.Contains({ 5, 3 }))
			exit_status |= 128;
		 
		// Length

		if (x.Length() >= 1.4f && x.Length() >= 1.43f)
			exit_status |= 128;

		// Normalize

		if (cast.Normalize() != Vector4<float>(0.793f))
			exit_status |= 128;

		if (cast.NormalizeThis() != Vector4<float>(0.793f))
			exit_status |= 128;

	}
	return exit_status;
}
