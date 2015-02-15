#pragma once 

#include <Matrix44.h>
#include <Vector3.h>
#include <Matrix33.h>

namespace Pro {
	namespace Graphics {
		using namespace Math;

		class Transformation {
		protected:
			Matrix44<float> trans_matrix_;
			Matrix33<float> x_matrix_;
			Matrix33<float> y_matrix_;
			Matrix33<float> z_matrix_;
			Matrix33<float> rotation_matrix_;
			Matrix44<float> normal_matrix_;
			Vector3<float> scale_;
			Vector3<float> position_;
			Vector3<float> rotation_;

			// true if matrix hasn't been changed
			bool is_processed_;
			bool is_normal_processed_;
			// false if no rotations applied since last frame
			bool is_rotated_; 

		public:
			Transformation();
			~Transformation() {};

			void scale(const Vector3<float>& scale);
			void rotation(const Vector3<float>& xyz);
			void position(const Vector3<float>& position);
			const Vector3<float>& position() const;
			const Vector3<float>& scale() const;
			const Vector3<float>& rotation() const;
			void move(const Vector3<float>& delta);
			void rotate(const Vector3<float>& xyz);
			const Matrix44<float>& getMatrix();
			const Matrix44<float>& getNormalMatrix();


			Transformation& operator=(const Transformation&);
			Transformation& operator=(Transformation&&);
		};
	}
}