#pragma once 

#include <Matrix44.h>
#include <Vector3.h>
#include <Matrix33.h>

namespace Pro {
	namespace Graphics {


		class Transformation {
		protected:
			Matrix44<float> trans_matrix;
			Matrix33<float> x_matrix;
			Matrix33<float> y_matrix;
			Matrix33<float> z_matrix;
			Matrix44<float> scale_matrix;
			Vector3<float> position;
			Vector3<float> rotation;

			// true if matrix hasn't been changed
			bool isProcessed;

		public:
			Transformation();
			~Transformation() {};

			void setScale(const Vector3<float>& scale);
			void setRotation(const Vector3<float>& xyz);
			void setPosition(const Vector3<float>& position);
			void move(const Vector3<float>& delta);
			void rotate(const Vector3<float>& xyz);
			Matrix44<float>& getViewMatrix();
		};
	}
}