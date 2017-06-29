#pragma once
#include <Vector3.h>
#include <Matrix44.h>
#include "../Core/Graphics/extern/lib3ds/lib3ds.h"

using namespace Pro;
using namespace Math;

class Plane {
public:
	Vector3<float> position_;
	Vector3<float> rotation_;
	Vector3<float> velocity_;

	float thrust_; // in kN
	float pitch_control_;
	float yaw_control_;
	float roll_control_;

	// constants
	const float max_roll_ = 4;
	const float max_yaw_ = 4;
	const float max_pitch_ = 4; 

	const float plane_weight = 20411; // 20.4 Tons 
	const float maximum_thrust = 259;
	const float drag_coefficient = 0.021f; // for a fighter jet
	const float lift_coefficient = 0.01f; // For a F-15 https://www.nasa.gov/centers/dryden/pdf/88337main_H-1999.pdf
	const float air_density = 1.25;
	const float wing_area = 56.48f; // http://www.globalsecurity.org/military/systems/aircraft/f-15-specs.htm
	const float reference_area = 0.00489f; //http://faculty.dwc.edu/sadraey/Chapter%203.%20Drag%20Force%20and%20its%20Coefficient.pdf
	const float speed_scale = 0.0001f;

	Lib3dsFile* model;
	std::vector<std::pair<int, int>> model_objects;
	GLuint vertex_buffer;

	Plane() : position_(0), rotation_(0), velocity_(0) { 
		thrust_ = 1; 
		pitch_control_ = yaw_control_ = roll_control_ = 0;
		model = nullptr;
		vertex_buffer = 0;
	}

	~Plane() {
		if(model != nullptr)
			lib3ds_file_free(model);
		if (vertex_buffer != 0)
			glDeleteBuffers(1, &vertex_buffer);
	}

	void Update() {
		// Apply thrust
		velocity_.z += (thrust_ * 1000) / plane_weight;
		 
		const float lift = lift_coefficient * (air_density * powf(velocity_.z, 2) / 2) * wing_area; 
		const float drag = drag_coefficient * reference_area * .5f * air_density * powf(velocity_.z, 2); 
		  
		// Apply action forces
		velocity_.y += lift / plane_weight; 

		// Apply reactive forces
		velocity_.y -= 9.8f;
		velocity_.z -= drag / plane_weight;

		// Limit the speed
		if (velocity_.y >= 0.5f)
			velocity_.y = 0.2f; 
		  
		// Apply control forces
		rotation_.x += pitch_control_;
		rotation_.y += yaw_control_;
		rotation_.z += roll_control_;	
		
		// Rotate the velocity vector to the velocity in world space
		Matrix44<float> rotation_matrix;
		rotation_matrix.Rotate<ROTATION_ORDER::XYZ>({ -rotation_.x, -rotation_.y, 0 }); 

		const Vector3<float> world_velocity = ToVector3(ToVector4(velocity_) * rotation_matrix); 
		position_ += (world_velocity * speed_scale); 

		if (position_.y <= 3) {
			position_.y = 3;
			rotation_.x = rotation_.z = 0;
			velocity_.y = 0; // We're on the ground, and therefor have no downwards velocity
		}

		return; // Skip printing the debug info

		std::cout << "------------------------------------------------------------" << std::endl;
		std::cout << "Drag: " << drag << " Lift: " << lift << std::endl;
		std::cout << "Local Velocity: x: "<< velocity_.x << " y: " << velocity_.y << " z: " << velocity_.z << std::endl;
		std::cout << "World Velocity: x: " << world_velocity.x << " y: " << world_velocity.y << " z: " << world_velocity.z << std::endl;
		std::cout << "Rotation: x: " << rotation_.x << " y: " << rotation_.y << " z: " << rotation_.z << std::endl;
		std::cout << "Position: x: " << position_.x << " y: " << position_.y << " z: " << position_.z << std::endl;
	}

	Matrix44<float> GetCamera() {
		Matrix44<float> camera;
		camera.Translate(-position_);
		//camera.Rotate<ROTATION_ORDER::XYZ>(rotation_);
		camera.Rotate({ rotation_.x, rotation_.y, 0 });
		camera.Rotate({ 0, 0, rotation_.z }); 
		return camera;
	}
	 
	void lowerPitch() { rotation_.x -= 0.01f; }
	void raisePitch() { rotation_.x += 0.01f; }
	void raiseThrust() { if (thrust_ <= maximum_thrust) thrust_ += 0.25f; }
	void lowerThrust() { if (thrust_ >= 1.f) thrust_ -= 0.25f; }
	void leftRoll() { rotation_.z -= 0.01f; }
	void rightRoll() { rotation_.z += 0.01f; }
	void leftYaw() { rotation_.y -= 0.01f; }
	void rightYaw() { rotation_.y += 0.01f; }

	const Vector3<float>& position() { return position_; }
	const Vector3<float>& rotation() { return rotation_; }
	const Vector3<float>& velocity() { return velocity_; }

	void position(const Vector3<float>& p) { position_ = p; }
	void rotation(const Vector3<float>& r) { rotation_ = r; }
	void velocity(const Vector3<float>& v) { velocity_ = v; }
};
