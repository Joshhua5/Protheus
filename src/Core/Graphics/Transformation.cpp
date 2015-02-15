#include "Transformation.h"

using namespace Pro;
using namespace Graphics;
using namespace Math;

Transformation::Transformation() {
	is_processed_ = false;
	is_normal_processed_ = false;
	is_rotated_ = true;
	rotation(Vector3<float>(0, 0, 0));
	position(Vector3<float>(0, 0, 0));
	scale({ 1, 1, 1 });
}

void Transformation::rotation(const Vector3<float>& xyz) {
	if (!is_rotated_ && xyz == rotation_)
		return;
	rotation_ = xyz;
	is_processed_ = false;
	is_rotated_ = true;

	x_matrix_.matrix_[0][0] = 1;
	x_matrix_.matrix_[1][0] = 0;
	x_matrix_.matrix_[2][0] = 0;
	x_matrix_.matrix_[0][1] = 0;
	x_matrix_.matrix_[1][1] = cos(xyz.x);
	x_matrix_.matrix_[2][1] = -sin(xyz.x);
	x_matrix_.matrix_[0][2] = 0;
	x_matrix_.matrix_[1][2] = sin(xyz.x);
	x_matrix_.matrix_[2][2] = cos(xyz.x);
			   
	y_matrix_.matrix_[0][0] = cos(xyz.y);
	y_matrix_.matrix_[1][0] = 0;
	y_matrix_.matrix_[2][0] = sin(xyz.y);
	y_matrix_.matrix_[0][1] = 0;
	y_matrix_.matrix_[1][1] = 1;
	y_matrix_.matrix_[2][1] = 0;
	y_matrix_.matrix_[0][2] = -sin(xyz.y);
	y_matrix_.matrix_[1][2] = 0;
	y_matrix_.matrix_[2][2] = cos(xyz.y);
			   
	z_matrix_.matrix_[0][0] = cos(xyz.z);
	z_matrix_.matrix_[1][0] = -sin(xyz.z);
	z_matrix_.matrix_[2][0] = 0;
	z_matrix_.matrix_[0][1] = sin(xyz.z);
	z_matrix_.matrix_[1][1] = cos(xyz.z);
	z_matrix_.matrix_[2][1] = 0;
	z_matrix_.matrix_[0][2] = 0;
	z_matrix_.matrix_[1][2] = 0;
	z_matrix_.matrix_[2][2] = 1;
} 

void Transformation::rotate(const Vector3<float>& xyz) {
	if (xyz.Equals(0, 0, 0))
		return;
	rotation_ += xyz;
	is_rotated_ = true;
	rotation(rotation_);
}

const Matrix44<float>& Transformation::getNormalMatrix() {
	if (!is_normal_processed_) {
		normal_matrix_ = getMatrix().Invert().Transpose();
		is_normal_processed_ = true;
	}
	return normal_matrix_;
}

const Matrix44<float>& Transformation::getMatrix() {
	if (is_processed_)
		return trans_matrix_;

	Matrix44<float> scale_matrix(IDENTITY_MATRIX);
	if (is_rotated_)
		rotation_matrix_ = x_matrix_ * y_matrix_ * z_matrix_;
	is_normal_processed_ = false;
	// Create final transformation matrix
	// Move rotation data into trans matrix 
	trans_matrix_.matrix_[0][0] = rotation_matrix_.matrix_[0][0];
	trans_matrix_.matrix_[1][0] = rotation_matrix_.matrix_[1][0];
	trans_matrix_.matrix_[2][0] = rotation_matrix_.matrix_[2][0];
	trans_matrix_.matrix_[0][1] = rotation_matrix_.matrix_[0][1];
	trans_matrix_.matrix_[1][1] = rotation_matrix_.matrix_[1][1];
	trans_matrix_.matrix_[2][1] = rotation_matrix_.matrix_[2][1];
	trans_matrix_.matrix_[0][2] = rotation_matrix_.matrix_[0][2];
	trans_matrix_.matrix_[1][2] = rotation_matrix_.matrix_[1][2];
	trans_matrix_.matrix_[2][2] = rotation_matrix_.matrix_[2][2];

	// move position data into trans matrix

	trans_matrix_.matrix_[3][0] = position_.x;
	trans_matrix_.matrix_[3][1] = position_.y;
	trans_matrix_.matrix_[3][2] = position_.z;
				 
	trans_matrix_.matrix_[0][3] = 0;
	trans_matrix_.matrix_[1][3] = 0;
	trans_matrix_.matrix_[2][3] = 0;
	trans_matrix_.matrix_[3][3] = 1;
				 
	// Scale	 
	scale_matrix.matrix_[0][0] = scale_.x;
	scale_matrix.matrix_[1][1] = scale_.y;
	scale_matrix.matrix_[2][2] = scale_.z;
	scale_matrix.matrix_[3][3] = 1;
	trans_matrix_ *= scale_matrix;

	// finished processing
	is_processed_ = true;

	return trans_matrix_;
}

void Transformation::position(const Vector3<float>& _position) {
	if (position_ == _position)
		return;
	// flag to reprocess
	is_processed_ = false;
	position_ = _position;
}


const Vector3<float>& Transformation::position() const {
	return position_;
}

void Transformation::move(const Vector3<float>& delta) {
	if (delta.Equals(0, 0, 0))
		return;
	// flag to reprocess
	is_processed_ = false;
	position_ += delta;
}

void Transformation::scale(const Vector3<float>& _scale) {
	if (scale_ == _scale)
		return;
	scale_ = _scale;
	is_processed_ = false;
}


const Vector3<float>&  Transformation::scale() const {
	return scale_;
}
const Vector3<float>&  Transformation::rotation() const {
	return rotation_;
}

Transformation& Transformation::operator=(const Transformation& rhs) {
	memcpy(this, &rhs, sizeof(Transformation));
	return *this;
}
Transformation& Transformation::operator=(Transformation&& rhs) {
	memcpy(this, &rhs, sizeof(Transformation));
	return *this;
}