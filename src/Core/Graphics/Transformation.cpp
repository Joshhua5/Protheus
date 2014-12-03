#include "Transformation.h"

using namespace Pro;
using namespace Graphics;

Transformation::Transformation() {
	isProcessed = false;
	setRotation(Vector3<float>(0, 0, 0));
	setPosition(Vector3<float>(0, 0, 0));
}


void Transformation::setRotation(const Vector3<float>& xyz) {
	rotation = xyz;
	isProcessed = false;

	x_matrix._m[0][0] = 1;
	x_matrix._m[1][0] = 0;
	x_matrix._m[2][0] = 0;
	x_matrix._m[0][1] = 0;
	x_matrix._m[1][1] = cos(xyz.x);
	x_matrix._m[2][1] = -sin(xyz.x);
	x_matrix._m[0][2] = 0;
	x_matrix._m[1][2] = sin(xyz.x);
	x_matrix._m[2][2] = cos(xyz.x);

	y_matrix._m[0][0] = cos(xyz.y);
	y_matrix._m[1][0] = 0;
	y_matrix._m[2][0] = sin(xyz.y);
	y_matrix._m[0][1] = 0;
	y_matrix._m[1][1] = 1;
	y_matrix._m[2][1] = 0;
	y_matrix._m[0][2] = -sin(xyz.y);
	y_matrix._m[1][2] = 0;
	y_matrix._m[2][2] = cos(xyz.y);

	z_matrix._m[0][0] = cos(xyz.z);
	z_matrix._m[1][0] = -sin(xyz.z);
	z_matrix._m[2][0] = 0;
	z_matrix._m[0][1] = sin(xyz.z);
	z_matrix._m[1][1] = cos(xyz.z);
	z_matrix._m[2][1] = 0;
	z_matrix._m[0][2] = 0;
	z_matrix._m[1][2] = 0;
	z_matrix._m[2][2] = 1; 
}

void Transformation::rotate(const Vector3<float>& xyz) {
	isProcessed = false;
	rotation += xyz;
	setRotation(rotation);
}

Matrix44<float>& Transformation::getViewMatrix() {
	if (isProcessed)
		return trans_matrix;

	Matrix33<float> rotation = x_matrix * y_matrix * z_matrix; 
	// Create final transformation matrix
	// Move rotation data into trans matrix

	trans_matrix._m[0][0] = rotation._m[0][0];
	trans_matrix._m[1][0] = rotation._m[1][0];
	trans_matrix._m[2][0] = rotation._m[2][0];
	trans_matrix._m[0][1] = rotation._m[0][1];
	trans_matrix._m[1][1] = rotation._m[1][1];
	trans_matrix._m[2][1] = rotation._m[2][1];
	trans_matrix._m[0][2] = rotation._m[0][2];
	trans_matrix._m[1][2] = rotation._m[1][2];
	trans_matrix._m[2][2] = rotation._m[2][2];

	// move position data into trans matrix

	trans_matrix._m[3][0] = position.x;
	trans_matrix._m[3][1] = position.y;
	trans_matrix._m[3][2] = position.z;

	trans_matrix._m[0][3] = 0;
	trans_matrix._m[1][3] = 0;
	trans_matrix._m[2][3] = 0;
	trans_matrix._m[3][3] = 1;

	// finished processing
	isProcessed = true;
	
	return trans_matrix; 
}

void Transformation::setPosition(const Vector3<float>& _position) {
	// flag to reprocess
	isProcessed = false;
	position = _position;
}
void Transformation::move(const Vector3<float>& delta) {
	// flag to reprocess
	isProcessed = false;
	position += delta;
}