
//#ifdef __SSE3

//#endif

#pragma once
namespace Pro{
	namespace Math{
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
