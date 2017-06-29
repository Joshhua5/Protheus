#pragma once
#include <Vector3.h>

using namespace Pro;
using namespace Math;

class Hoop {
	Vector3<float> position_;
	int radius;

public:
	Hoop(const Vector3<float>& pos, int rad) { position_ = pos; radius = rad; }

	bool HasCollided(Vector3<float> with) const {
		return
			with.x >= position_.x - radius && with.x <= position_.x + radius &&
			with.y >= position_.y - radius && with.y <= position_.y + radius &&
			with.z >= position_.z - radius && with.z <= position_.z + radius;
	}

	const Vector3<float>& position() const {
		return position_;
	}
};