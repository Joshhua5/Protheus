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
#include <memory>
#include "Error.h"
 
#pragma once

namespace Pro {
	namespace Math {
		template<typename T>
		class Matrix33
		{
		public:
			T _m[3][3];

		public:
			/*! Must have an initializer_list with 9 values */
			Matrix33(std::initializer_list<T> values) {
				if (values.size() == 9)
					memcpy(_m, values.begin(), sizeof(T) * 9);
				else
					error.reportErrorNR("Incorrect intitalizer_list passed to Matrix33 constructor.\0");
			}

			Matrix33(float values[9]) {
				_m[0][0] = values[0];
				_m[1][0] = values[1];
				_m[2][0] = values[2];
				_m[0][1] = values[3];
				_m[1][1] = values[4];
				_m[2][1] = values[5];
				_m[0][2] = values[6];
				_m[1][2] = values[7];
				_m[2][2] = values[8];
			}

			Matrix33(Matrix33&& rhs) {
				_m[0][0] = std::move(rhs._m[0][0]);
				_m[1][0] = std::move(rhs._m[1][0]);
				_m[2][0] = std::move(rhs._m[2][0]);
				_m[0][1] = std::move(rhs._m[0][1]);
				_m[1][1] = std::move(rhs._m[1][1]);
				_m[2][1] = std::move(rhs._m[2][1]);
				_m[0][2] = std::move(rhs._m[0][2]);
				_m[1][2] = std::move(rhs._m[1][2]);
				_m[2][2] = std::move(rhs._m[2][2]);
			}

			Matrix33(const Matrix33& rhs) {
				_m[0][0] = rhs._m[0][0];
				_m[1][0] = rhs._m[1][0];
				_m[2][0] = rhs._m[2][0];
				_m[0][1] = rhs._m[0][1];
				_m[1][1] = rhs._m[1][1];
				_m[2][1] = rhs._m[2][1];
				_m[0][2] = rhs._m[0][2];
				_m[1][2] = rhs._m[1][2];
				_m[2][2] = rhs._m[2][2];
			}

			/*! Sets all data to 0 */
			Matrix33() {
				memset(this, 0, sizeof(Matrix33));
			};

			~Matrix33() {};

			/*! T must contian a valid copy and -= operator */
			Matrix33 operator-(Matrix33& m) {
				Matrix33 o = *this;
				o -= m;
				return std::move(o);
			}

			/*! T must contian a valid move and -= operator */
			Matrix33 operator-(Matrix33&& m) {
				Matrix33 o = *this;
				o -= m;
				return std::move(o);
			}

			/*! T must contian a valid copy and += operator */
			Matrix33 operator+(Matrix33& m) {
				Matrix33 o = *this;
				o += m;
				return std::move(o);
			}

			/*! T must contian a valid move and += operator */
			Matrix33 operator+(Matrix33&& m) {
				Matrix33 o = *this;
				o += m;
				return std::move(o);
			}

			/*! T must contian a valid copy and *= operator */
			Matrix33 operator*(Matrix33& m) {
				Matrix33 o = *this;
				o *= m;
				return std::move(o);
			}

			/*! T must contian a valid move and *= operator */
			Matrix33 operator*(Matrix33&& m) {
				Matrix33 o = *this;
				o *= m;
				return std::move(o);
			}


			/*! T must contain a valid == and && operator*/
			bool operator==(Matrix33& m) {
				return
					_m[0][0] == m._m[0][0] &&
					_m[0][1] == m._m[0][1] &&
					_m[0][2] == m._m[0][2] &&

					_m[1][0] == m._m[1][0] &&
					_m[1][1] == m._m[1][1] &&
					_m[1][2] == m._m[1][2] &&

					_m[2][0] == m._m[2][0] &&
					_m[2][1] == m._m[2][1] &&
					_m[2][2] == m._m[2][2];
			}

			Matrix33& operator=(Matrix33& m) {
				memcpy(_m, m._m, sizeof(T) * 9);
				return *this;
			}

			Matrix33& operator=(Matrix33&& m) {
				memcpy(_m, m._m, sizeof(T) * 9);
				return *this;
			}

			/*! T must contain a valid -= operator */
			void operator-=(Matrix33& m) {
				for (char x = 0; x < 9; x++)
					_m[0][x] -= m._m[0][x];
			}

			/*! T must contain a valid += operator */
			void operator+=(Matrix33& m) {
				for (char x = 0; x < 9; x++)
					_m[0][x] += m._m[0][x];
			}

			/*! T must contain a valid *, + and = operators */
			void operator*=(Matrix33& m) {
				Matrix33<T> out(*this);
				for (char x = 0; x < 3; ++x) {
					for (char y = 0; y < 3; ++y)
						_m[x][y] =
						(out._m[0][y] * m._m[x][0]) +
						(out._m[1][y] * m._m[x][1]) +
						(out._m[2][y] * m._m[x][2]);
				}
			}

			/*! Transposes the current matrix, T must contain a valid Copy constructor and = operator */
			Matrix33 transpose() {
				// Copy the matrix
				Matrix33 o;
				// Flip the matrix
				for (char x = 0; x < 3; ++x)
					for (char y = 0; y < 3; ++y)
						o._m[x][y] = _m[y][x];
				return std::move(o);
			}

			/*! Returns the determinate, T must contain a valid *, =, -= and += operators */
			T determinate() {
				T det = 0;
				det = _m[0][0] * (_m[1][1] * _m[2][2] - _m[1][2] * _m[2][1]);
				det -= _m[0][1] * (_m[1][0] * _m[2][2] - _m[1][2] * _m[2][0]);
				det += _m[0][2] * (_m[1][0] * _m[2][1] - _m[1][1] * _m[2][0]);
				return det;
			}

		};
	}
}