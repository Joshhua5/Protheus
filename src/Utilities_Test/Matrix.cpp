#include <Graphics\Transformation.h> 
#include <Vector2.h>

using namespace Pro; 
using namespace Graphics;

int main() {
	// Matrix test

	float x[16] = { 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 4.0f, 0.0f, 0.0f, 2.0f, 0.0f, 4.0f }; 
	float b[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 2.0f, 2.0f, 4.0f };

	Matrix44<float> m1(b);
	Matrix44<float> m2(x);

	Matrix44<float> mulResult(m1 * m2); 
	 
	Matrix44<float> addResult(m1 + m2); 
 
	Matrix44<float> subResult(m1 - m2); 

	Matrix44<float> invert(m1.invert()); 
	 
	float det = Matrix44<float>({ 1.f, 2.f, 3.f, 4.f, 4.f, 5.f, 3.f, 6.f, 6.f, 5.f, 5.f, 7.f, 5.f, 5.f, 5.f, 4.f }).determinate();
	  
	Transformation trans;

	trans.setPosition({ 2, 2, 2 });
	trans.setScale({ 2, 2, 2 });
	trans.setRotation({ 90, 90, 90 });

	Matrix44<float> trans_mat = trans.getViewMatrix();
	
	return 0;
}