#include "AudioListener.h"

using namespace Pro;
using namespace Audio;
using namespace Math;

unsigned CAudioListener::active_listener_id;

unsigned& CAudioListener::getCount() {
	static unsigned listener_count = 0;
	return listener_count;
}

CAudioListener::CAudioListener() {
	if (getCount() == 0)
		active_listener_id = 0;   
	listener_id = getCount()++;
}

CAudioListener::~CAudioListener() {
	listener_id = -1;
}

CAudioListener::CAudioListener(const CAudioListener& other) {
	// Create a new object to get a new ID 
	this->gain_ = other.gain_;
	this->position_ = other.position_;
	this->orientation_ = other.orientation_;
	this->velocity_ = other.velocity_;
	this->listener_id = getCount()++;
}

CAudioListener::CAudioListener(CAudioListener&& other) {
	this->gain_ = other.gain_;
	this->position_ = other.position_;
	this->orientation_ = other.orientation_;
	this->velocity_ = other.velocity_;
	this->listener_id = other.listener_id;
	other.listener_id = -1;
}

CAudioListener& CAudioListener::operator=(const CAudioListener& rhs) {
	// Create a new object to get a new ID 
	this->gain_ = rhs.gain_;
	this->position_ = rhs.position_;
	this->orientation_ = rhs.orientation_;
	this->velocity_ = rhs.velocity_;
	this->listener_id = getCount()++;
	return *this;
}

CAudioListener& CAudioListener::operator=(CAudioListener&& rhs) {
	// Create a new object to get a new ID 
	this->gain_ = rhs.gain_;
	this->position_ = rhs.position_;
	this->orientation_ = rhs.orientation_;
	this->velocity_ = rhs.velocity_;
	this->listener_id = rhs.listener_id;
	rhs.listener_id = -1;
	return *this;
}

void CAudioListener::position(const Vector3<float>& v) {
	position_ = v;
	if (active_listener_id == listener_id)
		alListener3f(AL_POSITION, v.x, v.y, v.z);
}

void CAudioListener::setOrientation(const Vector3<float>& v) {
	orientation_ = v;
	if (active_listener_id == listener_id)
		alListener3f(AL_ORIENTATION, v.x, v.y, v.z);
}

void CAudioListener::velocity(const Vector3<float>& v) {
	velocity_ = v;
	if (active_listener_id == listener_id)
		alListener3f(AL_VELOCITY, v.x, v.y, v.z);
}

// Value between 1 and 0 with 1 being unattenuated
void CAudioListener::setGain(float v) {
	gain_ = v;
	if (active_listener_id == listener_id)
		alListenerf(AL_GAIN, v);
}

void CAudioListener::setActive() const {
	if (listener_id != -1 && listener_id != active_listener_id) {
		active_listener_id = listener_id;

		alListener3f(AL_POSITION, position_.x, position_.y, position_.z);
		alListener3f(AL_ORIENTATION, orientation_.x, orientation_.y, orientation_.z);
		alListener3f(AL_VELOCITY, velocity_.x, velocity_.y, velocity_.z);
		alListenerf(AL_GAIN, gain_);
	}
}