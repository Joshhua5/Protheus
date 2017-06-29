/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:12:2014 Waring J.

*************************************************************************/
#pragma once
#include "Matrix33.h"
#include "Vector3.h"
#include <initializer_list>
#include "Log.h"

namespace Pro {
	namespace Math {
		//! Constant global indentity matrix
		alignas(32) static float IDENTITY_MATRIX[] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
        
        enum struct ROTATION_ORDER {
            XYZ,
            ZYX
        };

		 
        class alignas(32) Matrix44 {
		public:
            float matrix_[4][4];

            Matrix44(const float values[4][4]) {
                _mm_store_ps(&matrix_[0][0], _mm_load_ps(&values[0][0]));
                _mm_store_ps(&matrix_[1][0], _mm_load_ps(&values[1][0]));
                _mm_store_ps(&matrix_[2][0], _mm_load_ps(&values[2][0]));
                _mm_store_ps(&matrix_[3][0], _mm_load_ps(&values[3][0]));
			}
            
            Matrix44(const float values[16]) {
                std::memcpy(matrix_, values, sizeof(float) * 16);
			}

			Matrix44(std::initializer_list<float> values) {
				if (values.size() >= 16)
					std::memcpy(matrix_, values.begin(), sizeof(float) * 16);
				else
                    global_log.Report<LogCode::FAULT>("Incorrect intitalizer_list passed to Matrix44 constructor.\0", __FUNCTION__, __LINE__);
			}
			Matrix44(const Matrix44& value) {
				std::memcpy(matrix_, value.matrix_, sizeof(float) * 16);
			}

			Matrix44(Matrix44&& value) {
				std::memcpy(matrix_, value.matrix_, sizeof(float) * 16);
			}

			/*! Initializes all elements of the matrix to the value */
			Matrix44(float value) { for (unsigned char x = 0; x < 16; ++x) matrix_[0][x] = value; }
            Matrix44() : Matrix44(IDENTITY_MATRIX){}
			~Matrix44() {}

			Matrix44 operator-(const Matrix44& m) const {
				Matrix44 o = *this;
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

			Matrix44 operator-(const float& m) const {
				Matrix44 o = *this;
				o -= m;
				return o;
			}
			Matrix44 operator+(const float& m) const {
				Matrix44 o = *this;
				o += m;
				return o;
			}
			Matrix44 operator*(const float& m) const {
				Matrix44 o = *this;
				o *= m;
				return o;
			}

			bool operator==(const Matrix44& m) const {
				if (
					matrix_[0][0] == m.matrix_[0][0] &&
					matrix_[0][1] == m.matrix_[0][1] &&
					matrix_[0][2] == m.matrix_[0][2] &&

					matrix_[1][0] == m.matrix_[1][0] &&
					matrix_[1][1] == m.matrix_[1][1] &&
					matrix_[1][2] == m.matrix_[1][2] &&

					matrix_[2][0] == m.matrix_[2][0] &&
					matrix_[2][1] == m.matrix_[2][1] &&
					matrix_[2][2] == m.matrix_[2][2])
					return true;
				return false;
			}
			void operator=(const Matrix44& m) {
				std::memcpy(matrix_, m.matrix_, sizeof(float) * 16);
			}

			void operator-=(const Matrix44& m) {
				for (char x = 0; x < 16; x++)
					matrix_[0][x] -= m.matrix_[0][x];
			}
			void operator+=(const Matrix44& m) {
				for (char x = 0; x < 16; x++)
					matrix_[0][x] += m.matrix_[0][x];
			}
			void operator*=(const Matrix44& m) { 
				Matrix44 copy = *this;
				for (unsigned char y = 0; y < 4; y++) {  
					for (unsigned char x = 0; x < 4; x++)
						matrix_[y][x] =
						(copy.matrix_[y][0] * m.matrix_[0][x]) +
						(copy.matrix_[y][1] * m.matrix_[1][x]) +
						(copy.matrix_[y][2] * m.matrix_[2][x]) +
						(copy.matrix_[y][3] * m.matrix_[3][x]);
				}
			}

			void operator/=(const float& m) {
				for (char y = 0; y < 16; y++)
					matrix_[0][y] /= m;
			}
			void operator-=(const float& m) {
				for (char y = 0; y < 16; y++)
					matrix_[0][y] -= m;
			}
			void operator+=(const float& m) {
				for (char y = 0; y < 16; y++)
					matrix_[0][y] += m;
			}
			void operator*=(const float& m) {
				for (char y = 0; y < 16; y++)
					matrix_[0][y] *= m;
			}

			/*! Transposes the matrix*/
			Matrix44 Transpose() {
				Matrix44 o;
				// Flip the matrix
				for (unsigned char x = 0; x < 4; x++)
					for (unsigned char y = 0; y < 4; y++)
						o.matrix_[x][y] = matrix_[y][x];

				// copy the matrix over
				return std::move(o);
			}

			/*! Returns the determinate */
			float Determinate() const {
				float det = 0;
				det =  matrix_[0][0] * Matrix33<float>({ matrix_[1][1], matrix_[2][1], matrix_[3][1], matrix_[1][2], matrix_[2][2], matrix_[3][2], matrix_[1][3], matrix_[2][3], matrix_[3][3] }).Determinate();
				det -= matrix_[0][1] * Matrix33<float>({ matrix_[1][0], matrix_[2][0], matrix_[3][0], matrix_[1][2], matrix_[2][2], matrix_[3][2], matrix_[1][3], matrix_[2][3], matrix_[3][3] }).Determinate();
				det += matrix_[0][2] * Matrix33<float>({ matrix_[1][0], matrix_[2][0], matrix_[3][0], matrix_[1][1], matrix_[2][1], matrix_[3][1], matrix_[1][3], matrix_[2][3], matrix_[3][3] }).Determinate();
				det -= matrix_[0][3] * Matrix33<float>({ matrix_[1][0], matrix_[2][0], matrix_[3][0], matrix_[1][1], matrix_[2][1], matrix_[3][1], matrix_[1][2], matrix_[2][2], matrix_[3][2] }).Determinate();
				return det;
			}

			/*! Inverts the Matrix44*/
			Matrix44 Invert() const {
				float inv[16], det;
				int i;
				float* m = (float*)matrix_;
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
					global_log.Report<LogCode::FAULT>("Unable to get invert of Matrix44, identity matrix returned instead.", __FUNCTION__, __LINE__);
					return Matrix44(IDENTITY_MATRIX);
				}

				det = 1.f / det;
				for (i = 0; i < 16; i++)
					out.matrix_[0][i] = inv[i] * det;

				return out;
			}
            
            template<ROTATION_ORDER RO = ROTATION_ORDER::ZYX>
            void Rotate(Vector3<float> xyz){
                const float cosx = cosf(xyz.x);
                const float cosy = cosf(xyz.y);
                const float cosz = cosf(xyz.z);
                
                const float sinx = sinf(xyz.x);
                const float siny = sinf(xyz.y);
                const float sinz = sinf(xyz.z);
 
                // The compiler should optimise out the branch
                // as the type is known at compile time
                if(RO == ROTATION_ORDER::ZYX){
                        *this *=
                    {
                        cosy * cosz, cosz * sinx * siny - cosx * sinz, cosx * cosz * siny + sinx * sinz, 0,
                        cosy * sinz, cosx * cosz + sinx * siny * sinz, -cosz * sinx + cosx * siny * sinz, 0,
                        -siny, cosy * sinx, cosx * cosy, 0,
                        0, 0, 0, 1
                    };
                }else{
                        *this *=
                    {
                        cosy * cosz, -cosy * sinz, siny, 0,
                        cosx * sinz + sinx * siny * cosz, cosx * cosz - sinx * siny * sinz, -sinx * cosy, 0,
                        sinx * sinz - cosx * siny * cosz, sinx * cosz + cosx * siny * sinz, cosx * cosy, 0,
                        0, 0, 0, 1
                    };
                }
            }
            
            void Translate(Vector3<float> xyz){
                matrix_[3][0] += xyz.x;
                matrix_[3][1] += xyz.y;
                matrix_[3][2] += xyz.z;
            }
            
            void Scale(Vector3<float> xyz){
                *this *= {
                    xyz.x, 0, 0, 0,
                    0, xyz.y, 0, 0,
                    0, 0, xyz.z, 0,
                    0, 0, 0, 1
                };
            }

			/*! Returns a pointer to the matrix */
			float* data() { return matrix_[0]; }

			const float* data() const { return &matrix_[0][0]; }
		};
	}
}