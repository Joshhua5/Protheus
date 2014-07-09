#include "Matrix33.h"

using namespace Pro;
using namespace Math;

Matrix33 Matrix33::operator/(Matrix33& m){
	Matrix33 o = *this;
	o /= m;
	return o;
}
Matrix33 Matrix33::operator-(Matrix33& m){
	Matrix33 o = *this;
	o -= m;
	return o;
}
Matrix33 Matrix33::operator+(Matrix33& m){
	Matrix33 o = *this;
	o += m;
	return o;
}
Matrix33 Matrix33::operator*(Matrix33& m){
	Matrix33 o = *this;
	o *= m;
	return o;
}

bool Matrix33::operator==(Matrix33& m){
#ifdef __SSE
	// SSE Code

	__m128 a1 = _mm_cmpeq_ps(_mm_loadu_ps(&_m[0][0]), _mm_loadu_ps(&m._m[0][0]));
	__m128 a2 = _mm_cmpeq_ps(_mm_loadu_ps(&_m[0][1]), _mm_loadu_ps(&m._m[0][1]));
	__m128 a3 = _mm_cmpeq_ps(_mm_loadu_ps(&_m[0][2]), _mm_loadu_ps(&m._m[0][2]));

	if ((a1.m128_u64[0] & a1.m128_u64[1]) == true &&
		(a2.m128_u64[0] & a2.m128_u64[1]) == true &&
		(a3.m128_u64[0] & a3.m128_u64[1]) == true)
		return true;
	return false;

#else
	// Scalar code

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
#endif
}
void Matrix33::operator=(Matrix33& m){
#ifdef __SSE
	// SSE Code
	_mm_storeu_ps(_m[0], _mm_loadu_ps(m._m[0]));
	_mm_storeu_ps(_m[3], _mm_loadu_ps(m._m[3]));
	__m128 p = _mm_loadu_ps(m._m[6]);
	_m[2][0] = p.m128_f32[0];
	_m[2][1] = p.m128_f32[1];
	_m[2][1] = p.m128_f32[2];
#else
	// Scalar code

	for (char x = 0; x < 9; ++x)
		*_m[x] += *m._m[x];

#endif
}

void Matrix33::operator/=(Matrix33& m){
#ifdef __SSE
	// SSE Code
	_mm_storeu_ps(_m[0], _mm_div_ps(_mm_loadu_ps(_m[0]), _mm_loadu_ps(m._m[0])));
	_mm_storeu_ps(_m[3], _mm_div_ps(_mm_loadu_ps(_m[3]), _mm_loadu_ps(m._m[3])));
	_mm_storeu_ps(_m[6], _mm_div_ps(_mm_loadu_ps(_m[6]), _mm_loadu_ps(m._m[6])));

	return;
#else
	// Scalar code
	for (char x = 0; x < 9; ++x)
		*_m[x] /= *m._m[x];
#endif
}

void Matrix33::operator-=(Matrix33& m){
#ifdef __SSE
	// SSE Code
	_mm_storeu_ps(_m[0], _mm_sub_ps(_mm_loadu_ps(_m[0]), _mm_loadu_ps(m._m[0])));
	_mm_storeu_ps(_m[3], _mm_sub_ps(_mm_loadu_ps(_m[3]), _mm_loadu_ps(m._m[3])));
	_mm_storeu_ps(_m[6], _mm_sub_ps(_mm_loadu_ps(_m[6]), _mm_loadu_ps(m._m[6])));

	return;
#else
	// Scalar code
	for (char x = 0; x < 9; ++x)
		*_m[x] += *m._m[x];
#endif
}
void Matrix33::operator+=(Matrix33& m){
#ifdef __SSE
	// SSE Code

	_mm_storeu_ps(_m[0], _mm_add_ps(_mm_loadu_ps(_m[0]), _mm_loadu_ps(m._m[0])));
	_mm_storeu_ps(_m[3], _mm_add_ps(_mm_loadu_ps(_m[3]), _mm_loadu_ps(m._m[3])));
	__m128 p =  _mm_add_ps(_mm_loadu_ps(_m[6]), _mm_loadu_ps(m._m[6]));
	_m[2][0] = p.m128_f32[0];
	_m[2][1] = p.m128_f32[1];
	_m[2][1] = p.m128_f32[2];

	return;
#else
	// Scalar code
	for (char x = 0; x < 9; ++x)
		*_m[x] += *m._m[x];
#endif
}
void Matrix33::operator*=(Matrix33& m){
#ifdef __SSE
	// SSE Code

	__m128 row[3] = {
		_mm_loadu_ps(&_m[0][0]),
		_mm_loadu_ps(&_m[0][1]),
		_mm_loadu_ps(&_m[0][2]) };

	// Flip Matrix m

	__m128 col[3];

	for (char x = 0; x < 3; ++x){
		col[0].m128_f32[x] = _m[0][x];
		col[1].m128_f32[x] = _m[1][x];
		col[2].m128_f32[x] = _m[2][x];
	}

	for (char y = 0; y < 3; y++)
		for (char x = 0; x < 3; y++){
		__m128 ans = _mm_mul_ps(row[y], col[x]);
		_m[x][y] = ans.m128_f32[0] + ans.m128_f32[1] + ans.m128_f32[2];
		}

	return;

#else
	// Scalar code

	for (char y = 0; y < 3; y++){
		float* row = &_m[0][y];
		for (char x = 0; x < 3; y++)
			row[x] =
			(row[x] * m._m[x][0]) +
			(row[x] * m._m[x][1]) +
			(row[x] * m._m[x][2]);
	}
	return;
#endif
}

void Matrix33::transpose(){
#ifdef __SSE
	// SSE Code
	__m128 col[3];

	for (char x = 0; x < 3; ++x){
		col[0].m128_f32[x] = _m[0][x];
		col[1].m128_f32[x] = _m[1][x];
		col[2].m128_f32[x] = _m[2][x];
	}

	_mm_storeu_ps(&_m[0][0], col[0]);
	_mm_storeu_ps(&_m[1][0], col[1]);
	// must manually store col[2] otherwise it'll overflow.
	_m[2][0] = col[2].m128_f32[0];
	_m[2][1] = col[2].m128_f32[1];
	_m[2][2] = col[2].m128_f32[2];

	return;
#else
	// Scalar code
	// Can't overrise the origin until
	// the flip has finished
	Matrix33 o;
	// Flip the matrix
	for (char x = 0; x < 3; ++x)
		for (char y = 0; y < 3; y++)
			o._m[x][y] = _m[y][x];

	// copy the matrix over
	*this = o;
	return;
#endif
}