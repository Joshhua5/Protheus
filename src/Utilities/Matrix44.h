/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:12:2014 Waring J.

*************************************************************************/
#pragma once
#include "Matrix33.h"
#include "Error.h"

namespace Pro {

	static float identity_matrix[] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	}; 

	template<typename T>
	class Matrix44 {
	public:
		T _m[4][4];

		Matrix44(T values[4][4]) {
			memcpy(_m, values, sizeof(T) * 16);
		}
		Matrix44(T values[16]) {
			memcpy(_m, values, sizeof(T) * 16);
		}

		Matrix44(std::initializer_list<T> values) {
			if (values.size() == 16)
				memcpy(_m, values.begin(), sizeof(T) * 16);
			else
				error.reportErrorNR("Incorrect intitalizer_list passed to Matrix44 constructor.\0");
		}
		Matrix44(const Matrix44& value) {
			memcpy(_m, value._m, sizeof(T) * 16);
		}

		Matrix44(Matrix44&& value) {
			memcpy(_m, value._m, sizeof(T) * 16);
		}

		/*! Initializes all elements of the matrix to the value */
		Matrix44(T value) { for (char x = 0; x < 16; ++x) _m[0][x] = value; }
		Matrix44() {}
		~Matrix44() {}

		Matrix44 operator-(const Matrix44& m) const {
			Matrix44<float>o = *this;
			o -= m;
			return o;
		}

		Matrix44 operator+(const Matrix44& m) const {
			Matrix44 o = *this;
			o += m;
			return o;
		}

		Matrix44 operator*(const Matrix44& m) const {
			Matrix44 o = *this;
			o *= m;
			return o;
		}

		Matrix44 operator-(const T& m) const {
			Matrix44 o = *this;
			o -= m;
			return o;
		}
		Matrix44 operator+(const T& m) const {
			Matrix44<T>o = *this;
			o += m;
			return o;
		}
		Matrix44 operator*(const T& m) const {
			Matrix44<T>o = *this;
			o *= m;
			return o;
		}

		bool operator==(const Matrix44& m) const {
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
			memcpy(_m, m._m, sizeof(T) * 16);
		}

		void operator-=(const Matrix44& m) {
			for (char x = 0; x < 16; x++)
				_m[0][x] -= m._m[0][x];
		}
		void operator+=(const Matrix44& m) {
			for (char x = 0; x < 16; x++)
				_m[0][x] += m._m[0][x];
		}
		void operator*=(const Matrix44& m) {
			Matrix44 copy = *this;
			for (char y = 0; y < 4; y++) {
				float* row = &copy._m[y][0];
				float* out = &_m[y][0];
				for (char x = 0; x < 4; x++)
					out[x] =
					(row[0] * m._m[0][x]) +
					(row[1] * m._m[1][x]) +
					(row[2] * m._m[2][x]) +
					(row[3] * m._m[3][x]);
			}
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

		/*! Transposes the matrix*/
		Matrix44 transpose() {
			Matrix44 o;
			// Flip the matrix
			for (char x = 0; x < 4; x++)
				for (char y = 0; y < 4; y++)
					o._m[x][y] = _m[y][x];

			// copy the matrix over
			return std::move(o);
		}

		/*! Returns the determinate */
		T determinate() const {
			T det = 0;
			det = _m[0][0] * Matrix33<T>({ _m[1][1], _m[2][1], _m[3][1], _m[1][2], _m[2][2], _m[3][2], _m[1][3], _m[2][3], _m[3][3] }).determinate();
			det -= _m[0][1] * Matrix33<T>({ _m[1][0], _m[2][0], _m[3][0], _m[1][2], _m[2][2], _m[3][2], _m[1][3], _m[2][3], _m[3][3] }).determinate();
			det += _m[0][2] * Matrix33<T>({ _m[1][0], _m[2][0], _m[3][0], _m[1][1], _m[2][1], _m[3][1], _m[1][3], _m[2][3], _m[3][3] }).determinate();
			det -= _m[0][3] * Matrix33<T>({ _m[1][0], _m[2][0], _m[3][0], _m[1][1], _m[2][1], _m[3][1], _m[1][2], _m[2][2], _m[3][2] }).determinate();
			return det;
		}

		/*! Inverts the Matrix44*/
		Matrix44 invert() const {  
			float inv[16], det;
			int i;
			float* m = (float*)_m;
			Matrix44 out;

			inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
			inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
			inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
			inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
			inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
			inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
			inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
			inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
			inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
			inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
			inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
			inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
			inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
			inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
			inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
			inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

			det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

			if (det <= FLT_EPSILON && det >= -FLT_EPSILON) {
				error.reportMessage("Unable to get invert of Matrix44, identity matrix returned instead.\0");
				return Matrix44(identity_matrix);
			}

			det = 1.f / det;  
			for (i = 0; i < 16; i++)
				out._m[0][i] = inv[i] * det;

			return out; 
		}

		/*! Returns a pointer to the matrix */
		T* data() { return _m; }
	};
}