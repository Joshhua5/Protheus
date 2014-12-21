#pragma once

#include <Matrix44.h>
#include <ProMath.h>

namespace Pro {
	namespace Graphics {
		using namespace Math;

		class Projection {
			Matrix44<float> m_matrix;

			bool m_has_calculated = false;
			// -1 if unset, 0 if ortho, 1 if perspective
			char is_ortho = -1;
			float m_near;
			float m_far;
			float m_fov;
			float m_aspect;
		public:
			Projection(float near, float far, float fov, float aspect);
			void setNear(float);
			void setFar(float);
			void setAspect(float);
			void setFOV(float);

			const Matrix44<float>& getPerspective();
			const Matrix44<float>& getOrtho();
		};
	}
}