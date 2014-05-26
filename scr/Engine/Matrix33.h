
#ifdef __SSE
#include <xmmintrin.h>
#endif

#pragma once
namespace Pro{
	namespace Math{
#ifdef _MSC_VER
		__declspec(align(16))
#endif
		struct Matrix33
		{
		public: 
			float _m[3][3];

		public:
			Matrix33(){};
			~Matrix33(){};

			Matrix33 operator/(Matrix33&);
			Matrix33 operator-(Matrix33&);
			Matrix33 operator+(Matrix33&);
			Matrix33 operator*(Matrix33&);

			bool operator==(Matrix33&);
			void operator=(Matrix33&);

			void operator/=(Matrix33&);
			void operator-=(Matrix33&);
			void operator+=(Matrix33&);
			void operator*=(Matrix33&);

			void transpose();
		};
	}
}
