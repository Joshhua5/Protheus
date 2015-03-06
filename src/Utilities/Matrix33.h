/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:12:2014 Waring J.

*************************************************************************/
#pragma once
#include <memory>
#include "Log.h"
 
#pragma once

namespace Pro {
	namespace Math {
		template<typename T>
		class Matrix33
		{
		public:
			T matrix_[3][3];

		public:
			/*! Must have an initializer_list with 9 values */
			Matrix33(std::initializer_list<T> values) {
				if (values.size() == 9)
					memcpy(matrix_, values.begin(), sizeof(T) * 9);
				else
					global_log.ReportErrorNR("Incorrect intitalizer_list passed to Matrix33 constructor.\0");
			}

			Matrix33(float values[9]) {
				matrix_[0][0] = values[0];
				matrix_[1][0] = values[1];
				matrix_[2][0] = values[2];
				matrix_[0][1] = values[3];
				matrix_[1][1] = values[4];
				matrix_[2][1] = values[5];
				matrix_[0][2] = values[6];
				matrix_[1][2] = values[7];
				matrix_[2][2] = values[8];
			}

			Matrix33(Matrix33&& rhs) {
				matrix_[0][0] = std::move(rhs.matrix_[0][0]);
				matrix_[1][0] = std::move(rhs.matrix_[1][0]);
				matrix_[2][0] = std::move(rhs.matrix_[2][0]);
				matrix_[0][1] = std::move(rhs.matrix_[0][1]);
				matrix_[1][1] = std::move(rhs.matrix_[1][1]);
				matrix_[2][1] = std::move(rhs.matrix_[2][1]);
				matrix_[0][2] = std::move(rhs.matrix_[0][2]);
				matrix_[1][2] = std::move(rhs.matrix_[1][2]);
				matrix_[2][2] = std::move(rhs.matrix_[2][2]);
			}

			Matrix33(const Matrix33& rhs) {
				matrix_[0][0] = rhs.matrix_[0][0];
				matrix_[1][0] = rhs.matrix_[1][0];
				matrix_[2][0] = rhs.matrix_[2][0];
				matrix_[0][1] = rhs.matrix_[0][1];
				matrix_[1][1] = rhs.matrix_[1][1];
				matrix_[2][1] = rhs.matrix_[2][1];
				matrix_[0][2] = rhs.matrix_[0][2];
				matrix_[1][2] = rhs.matrix_[1][2];
				matrix_[2][2] = rhs.matrix_[2][2];
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
					matrix_[0][0] == m.matrix_[0][0] &&
					matrix_[0][1] == m.matrix_[0][1] &&
					matrix_[0][2] == m.matrix_[0][2] &&

					matrix_[1][0] == m.matrix_[1][0] &&
					matrix_[1][1] == m.matrix_[1][1] &&
					matrix_[1][2] == m.matrix_[1][2] &&

					matrix_[2][0] == m.matrix_[2][0] &&
					matrix_[2][1] == m.matrix_[2][1] &&
					matrix_[2][2] == m.matrix_[2][2];
			}

			Matrix33& operator=(Matrix33& m) {
				memcpy(matrix_, m.matrix_, sizeof(T) * 9);
				return *this;
			}

			Matrix33& operator=(Matrix33&& m) {
				memcpy(matrix_, m.matrix_, sizeof(T) * 9);
				return *this;
			}

			/*! T must contain a valid -= operator */
			void operator-=(Matrix33& m) {
				for (char x = 0; x < 9; x++)
					matrix_[0][x] -= m.matrix_[0][x];
			}

			/*! T must contain a valid += operator */
			void operator+=(Matrix33& m) {
				for (char x = 0; x < 9; x++)
					matrix_[0][x] += m.matrix_[0][x];
			}

			/*! T must contain a valid *, + and = operators */
			void operator*=(Matrix33& m) {
				Matrix33<T> out(*this);
				for (char x = 0; x < 3; ++x) {
					for (char y = 0; y < 3; ++y)
						matrix_[x][y] =
						(out.matrix_[0][y] * m.matrix_[x][0]) +
						(out.matrix_[1][y] * m.matrix_[x][1]) +
						(out.matrix_[2][y] * m.matrix_[x][2]);
				}
			}

			/*! Transposes the current matrix, T must contain a valid Copy constructor and = operator */
			Matrix33 Transpose() {
				// Copy the matrix
				Matrix33 o;
				// Flip the matrix
				for (char x = 0; x < 3; ++x)
					for (char y = 0; y < 3; ++y)
						o.matrix_[x][y] = matrix_[y][x];
				return std::move(o);
			}

			/*! Returns the determinate, T must contain a valid *, =, -= and += operators */
			T Determinate() {
				T det = 0;
				det = matrix_[0][0] * (matrix_[1][1] * matrix_[2][2] - matrix_[1][2] * matrix_[2][1]);
				det -= matrix_[0][1] * (matrix_[1][0] * matrix_[2][2] - matrix_[1][2] * matrix_[2][0]);
				det += matrix_[0][2] * (matrix_[1][0] * matrix_[2][1] - matrix_[1][1] * matrix_[2][0]);
				return det;
			}

		};
	}
}