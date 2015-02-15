#pragma once

#include <Matrix44.h>
#include <ProMath.h>

namespace Pro {
	namespace Graphics {
		using namespace Math;

		class Projection {
			Matrix44<float> matrix_;

			bool has_calculated_ = false;
			// -1 if unset, 0 if ortho, 1 if perspective
			char is_ortho_ = -1;
			float near_;
			float far_;
			float fov_;
			float aspect_;
		public:
			Projection(float near, float far, float fov, float aspect);
			void SetNear(float);
			void SetFar(float);
			void SetAspect(float);
			void SetFOV(float);

			const Matrix44<float>& GetPerspective();
			const Matrix44<float>& GetOrtho();
		};
	}
}