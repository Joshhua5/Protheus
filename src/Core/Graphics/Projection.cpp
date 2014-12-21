#include "Projection.h"

using namespace Pro;
using namespace Graphics;
using namespace Math;

Projection::Projection(float near, float far, float fov, float aspect) {
	m_near = near;
	m_far = far;
	m_fov = fov;
	m_aspect = aspect;
	m_has_calculated = false;

	m_matrix = Matrix44<float>(0.0f);
}

void Projection::setNear(float value) {
	if (m_near == value)
		return;
	m_near = value;
	m_has_calculated = false;
}

void Projection::setFar(float value) {
	if (m_far == value)
		return;
	m_far = value;
	m_has_calculated = false;
}

void Projection::setAspect(float value) {
	if (m_aspect == value)
		return;
	m_aspect = value;
	m_has_calculated = false;
}

void Projection::setFOV(float value) {
	if (m_fov == value)
		return;
	m_fov = value;
	m_has_calculated = false;
}


const Matrix44<float>& Projection::getOrtho() {
	if (m_has_calculated && is_ortho == 0)
		return m_matrix;

	return m_matrix;
}

const Matrix44<float>& Projection::getPerspective(){
	if (m_has_calculated && is_ortho == 1)
		return m_matrix;
	 
	// convert degree's to radians
	float fov = Math::degToRad(m_fov); 
	float w, h, Q;

	w = static_cast<float>(1.0f / tan(fov * 0.5)) / m_aspect;
	h = static_cast<float>(1.0f / tan(fov * 0.5));
	Q = m_far - (m_far - m_near);

	m_matrix._m[0][0] = w;
	m_matrix._m[1][1] = h;
	m_matrix._m[2][2] = Q;
	m_matrix._m[3][2] = -Q * m_near;
	m_matrix._m[2][3] = 1;

	m_has_calculated = true;
	is_ortho = 1;
	return m_matrix;
}