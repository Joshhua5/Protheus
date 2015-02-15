#include "Projection.h"

using namespace Pro;
using namespace Graphics;
using namespace Math;

Projection::Projection(float near, float far, float fov, float aspect) {
	near_ = near;
	far_ = far;
	fov_ = fov;
	aspect_ = aspect;
	has_calculated_ = false;

	matrix_ = Matrix44<float>(0.0f);
}

void Projection::SetNear(float value) {
	if (near_ == value)
		return;
	near_ = value;
	has_calculated_ = false;
}

void Projection::SetFar(float value) {
	if (far_ == value)
		return;
	far_ = value;
	has_calculated_ = false;
}

void Projection::SetAspect(float value) {
	if (aspect_ == value)
		return;
	aspect_ = value;
	has_calculated_ = false;
}

void Projection::SetFOV(float value) {
	if (fov_ == value)
		return;
	fov_ = value;
	has_calculated_ = false;
}


const Matrix44<float>& Projection::GetOrtho() {
	if (has_calculated_ && is_ortho_ == 0)
		return matrix_;

	return matrix_;
}

const Matrix44<float>& Projection::GetPerspective(){
	if (has_calculated_ && is_ortho_ == 1)
		return matrix_;
	 
	// convert degree's to radians
	float fov = Math::DegToRad(fov_); 
	float w, h, Q;

	w = static_cast<float>(1.0f / tan(fov * 0.5)) / aspect_;
	h = static_cast<float>(1.0f / tan(fov * 0.5));
	Q = far_ - (far_ - near_);

	matrix_.matrix_[0][0] = w;
	matrix_.matrix_[1][1] = h;
	matrix_.matrix_[2][2] = Q;
	matrix_.matrix_[3][2] = -Q * near_;
	matrix_.matrix_[2][3] = 1;

	has_calculated_ = true;
	is_ortho_ = 1;
	return matrix_;
}