#pragma once
#include "Matrix33.h"


namespace Pro {
	template<typename T>
	class Matrix44 {
	public:
		T _m[4][4];

		Matrix44(T values[4][4]) {
			memcpy(_m, values, sizeof(float) * 16);
		}
		Matrix44(T values[16]) {
			memcpy(_m, values, sizeof(float) * 16);
		}

		Matrix44() {};
		~Matrix44() {};

		Matrix44& operator/(const Matrix44& m) {
			Matrix44<float>o = *this;
			o /= m;
			return o;
		}
		Matrix44& operator-(const Matrix44& m) {
			Matrix44<float>o = *this;
			o -= m;
			return o;
		}
		Matrix44& operator+(const Matrix44& m) {
			Matrix44<float>o = *this;
			o += m;
			return o;
		}
		Matrix44& operator*(const Matrix44& m) {
			Matrix44<T>o = *this;
			o *= m;
			return o;
		}
		Matrix44& operator/(const T& m) {
			Matrix44<T>o = *this;
			o /= m;
			return o;
		}
		Matrix44& operator-(const T& m) {
			Matrix44<T>o = *this;
			o -= m;
			return o;
		}
		Matrix44& operator+(const T& m) {
			Matrix44<T>o = *this;
			o += m;
			return o;
		}
		Matrix44& operator*(const T& m) {
			Matrix44<T>o = *this;
			o *= m;
			return o;
		}

		bool operator==(const Matrix44& m) {

			if (
				_m[0][0] == m._m[0][0] &&
				_m[0][1] == m._m[0][1] &&
				_m[0][2] == m._m[0][2] &&

				_m[1][0] == m._m[1][0] &&
				_m[1][1] == m._m[1][1] &&
				_m[1][2] == m._m[1][2] &&

				_m[2][0] == m._m[2][0] &&
				_m[2][1] == m._m[2][1] &&
				_m[2][2] == m._m[2][2])
				return true;
			return false;
		}
		void operator=(const Matrix44& m) {
			for (char x = 0; x < 9; x++)
				*_m[x] += *m._m[x];
		}
		void operator/=(const Matrix44& m) {
			*this = (1 / determinate()) * invert(); 
		}
		void operator-=(const Matrix44& m) {
			for (char x = 0; x < 16; x++)
				*_m[x] += *m._m[x];
		}
		void operator+=(const Matrix44& m) {
			for (char x = 0; x < 16; x++)
				*_m[x] += *m._m[x];
		}
		void operator*=(const Matrix44& m) {
			for (char y = 0; y < 4; y++) {
				float* row = &_m[0][y];
				for (char x = 0; x < 4; y++)
					row[x] =
					(row[0] * m._m[x][0]) +
					(row[1] * m._m[x][1]) +
					(row[2] * m._m[x][2]) +
					(row[3] * m._m[x][3]);
			}
			return;
		}

		void operator/=(const T& m) {
			for (char y = 0; y < 16; y++)
				_m[0][y] /= m;
		}
		void operator-=(const T& m) {
			for (char y = 0; y < 16; y++)
				_m[0][y] -= m;
		}
		void operator+=(const T& m) {
			for (char y = 0; y < 16; y++)
				_m[0][y] += m;
		}
		void operator*=(const T& m) {
			for (char y = 0; y < 16; y++)
				_m[0][y] *= m;
		}


		void transpose() {
			Matrix44<float>o;
			// Flip the matrix
			for (char x = 0; x < 4; x++)
				for (char y = 0; y < 4; y++)
					o._m[x][y] = _m[y][x];

			// copy the matrix over
			*this = o;
			return;
		}

		T determinate() {
			T det = 0;
			det = _m[1][0] * Matrix33<T>({ _m[1][1], _m[1][2], _m[1][3], _m[2][1], _m[2][2], _m[2][3], _m[3][1], _m[3][2], _m[3][3] }).determinate();
			det -= _m[0][0] * Matrix33<T>({ _m[0][1], _m[0][2], _m[0][3], _m[2][1], _m[2][2], _m[2][3], _m[3][1], _m[3][2], _m[3][3] }).determinate();
			det += _m[0][0] * Matrix33<T>({ _m[0][1], _m[0][2], _m[0][3], _m[1][1], _m[1][2], _m[1][3], _m[3][1], _m[3][2], _m[3][3] }).determinate();
			det -= _m[0][0] * Matrix33<T>({ _m[0][1], _m[0][2], _m[0][3], _m[1][1], _m[1][2], _m[1][3], _m[2][1], _m[2][2], _m[2][3] }).determinate();
			return det;
		}

		Matrix44 invert() {
			return Matrix44<T>({
				// http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche23.html
				(_m[1][1] * _m[2][2] * _m[3][3] + _m[1][2] * _m[2][3] * _m[3][1] + _m[1][3] * _m[2][1] * _m[3][2] - _m[1][1] * _m[2][3] * _m[3][2] - _m[3][1] * _m[2][1] * _m[3][3] - _m[1][3] * _m[2][2] * _m[3][1]),
				(_m[0][1] * _m[2][3] * _m[3][2] + _m[0][2] * _m[2][1] * _m[3][3] + _m[0][3] * _m[2][2] * _m[3][1] - _m[0][1] * _m[2][2] * _m[3][3] - _m[0][2] * _m[2][3] * _m[3][1] - _m[0][3] * _m[2][1] * _m[3][2]),
				(_m[0][1] * _m[1][2] * _m[3][3] + _m[0][2] * _m[1][3] * _m[3][1] + _m[0][3] * _m[1][1] * _m[3][2] - _m[0][1] * _m[1][3] * _m[3][2] - _m[0][2] * _m[1][1] * _m[3][3] - _m[0][3] * _m[1][2] * _m[3][1]),
				(_m[0][1] * _m[1][3] * _m[2][2] + _m[0][2] * _m[1][1] * _m[2][3] + _m[0][3] * _m[1][2] * _m[2][1] - _m[0][1] * _m[1][2] * _m[2][3] - _m[0][2] * _m[1][3] * _m[2][1] - _m[0][3] * _m[1][1] * _m[2][2]),
				(_m[1][0] * _m[2][3] * _m[3][2] + _m[1][2] * _m[2][0] * _m[3][3] + _m[1][3] * _m[2][2] * _m[3][0] - _m[1][0] * _m[2][2] * _m[3][3] - _m[1][2] * _m[2][3] * _m[3][0] - _m[1][3] * _m[2][0] * _m[3][2]),
				(_m[0][0] * _m[2][2] * _m[3][3] + _m[0][2] * _m[2][3] * _m[3][0] + _m[0][3] * _m[2][0] * _m[3][2] - _m[0][0] * _m[2][3] * _m[3][2] - _m[0][2] * _m[2][0] * _m[3][3] - _m[0][3] * _m[2][2] * _m[3][0]),
				(_m[0][0] * _m[1][3] * _m[3][2] + _m[0][2] * _m[1][0] * _m[3][3] + _m[0][3] * _m[1][2] * _m[3][0] - _m[0][0] * _m[1][2] * _m[3][3] - _m[0][2] * _m[1][3] * _m[3][0] - _m[0][3] * _m[1][0] * _m[3][2]),
				(_m[0][0] * _m[1][2] * _m[2][3] + _m[0][2] * _m[1][3] * _m[2][0] + _m[0][3] * _m[1][0] * _m[2][2] - _m[0][0] * _m[1][3] * _m[2][2] - _m[0][2] * _m[1][0] * _m[2][3] - _m[0][3] * _m[1][2] * _m[2][0]),
				(_m[1][0] * _m[2][1] * _m[3][3] + _m[1][1] * _m[2][3] * _m[3][0] + _m[1][3] * _m[2][0] * _m[3][1] - _m[1][0] * _m[2][3] * _m[3][1] - _m[1][1] * _m[2][0] * _m[3][3] - _m[1][3] * _m[2][1] * _m[3][0]),
				(_m[0][0] * _m[2][3] * _m[3][1] + _m[0][1] * _m[2][0] * _m[3][3] + _m[0][3] * _m[2][1] * _m[3][0] - _m[0][0] * _m[2][1] * _m[3][3] - _m[0][1] * _m[2][3] * _m[3][0] - _m[0][3] * _m[2][0] * _m[3][1]),
				(_m[0][0] * _m[1][1] * _m[3][3] + _m[0][1] * _m[1][3] * _m[3][0] + _m[0][3] * _m[1][0] * _m[3][1] - _m[0][0] * _m[1][3] * _m[3][1] - _m[0][1] * _m[1][0] * _m[3][3] - _m[0][3] * _m[1][1] * _m[3][0]),
				(_m[0][0] * _m[1][3] * _m[2][1] + _m[0][1] * _m[1][0] * _m[2][3] + _m[0][3] * _m[1][1] * _m[2][0] - _m[0][0] * _m[1][1] * _m[2][3] - _m[0][1] * _m[1][3] * _m[2][0] - _m[0][3] * _m[1][0] * _m[2][1]),
				(_m[1][0] * _m[2][2] * _m[3][1] + _m[1][1] * _m[2][0] * _m[3][2] + _m[1][2] * _m[2][1] * _m[3][0] - _m[1][0] * _m[2][1] * _m[3][2] - _m[1][1] * _m[2][2] * _m[3][0] - _m[1][2] * _m[2][0] * _m[3][1]),
				(_m[0][0] * _m[2][1] * _m[3][2] + _m[0][1] * _m[2][2] * _m[3][0] + _m[0][2] * _m[2][0] * _m[3][1] - _m[0][0] * _m[2][2] * _m[3][1] - _m[0][1] * _m[2][0] * _m[3][2] - _m[0][2] * _m[2][1] * _m[3][0]),
				(_m[0][0] * _m[1][2] * _m[3][1] + _m[0][1] * _m[1][0] * _m[3][2] + _m[0][2] * _m[1][1] * _m[3][0] - _m[0][0] * _m[1][1] * _m[3][2] - _m[0][1] * _m[1][2] * _m[3][0] - _m[0][2] * _m[1][0] * _m[3][1]),
				(_m[0][0] * _m[1][1] * _m[2][2] + _m[0][1] * _m[1][2] * _m[2][0] + _m[0][2] * _m[1][0] * _m[2][1] - _m[0][0] * _m[1][2] * _m[2][1] - _m[0][1] * _m[1][0] * _m[2][2] - _m[0][2] * _m[1][1] * _m[2][0])
			}) * (1 / determinate());
		}

		T* data() { return _m; }
	};
}