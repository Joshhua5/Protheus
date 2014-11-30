#include "AudioListener.h"

using namespace Pro;
using namespace Audio;

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
	this->gain = other.gain;
	this->position = other.position;
	this->orientation = other.orientation;
	this->velocity = other.velocity;
	this->listener_id = getCount()++;
}

CAudioListener::CAudioListener(CAudioListener&& other) {
	this->gain = other.gain;
	this->position = other.position;
	this->orientation = other.orientation;
	this->velocity = other.velocity;
	this->listener_id = other.listener_id;
	other.listener_id = -1;
}

CAudioListener& CAudioListener::operator=(const CAudioListener& rhs) {
	// Create a new object to get a new ID 
	this->gain = rhs.gain;
	this->position = rhs.position;
	this->orientation = rhs.orientation;
	this->velocity = rhs.velocity;
	this->listener_id = getCount()++;
	return *this;
}

CAudioListener& CAudioListener::operator=(CAudioListener&& rhs) {
	// Create a new object to get a new ID 
	this->gain = rhs.gain;
	this->position = rhs.position;
	this->orientation = rhs.orientation;
	this->velocity = rhs.velocity;
	this->listener_id = rhs.listener_id;
	rhs.listener_id = -1;
	return *this;
}

void CAudioListener::setPosition(const Vector3& v) {
	position = v;
	if (active_listener_id == listener_id)
		alListener3f(AL_POSITION, v.x, v.y, v.z);
}

void CAudioListener::setOrientation(const Vector3& v) {
	orientation = v;
	if (active_listener_id == listener_id)
		alListener3f(AL_ORIENTATION, v.x, v.y, v.z);
}

void CAudioListener::setVelocity(const Vector3& v) {
	velocity = v;
	if (active_listener_id == listener_id)
		alListener3f(AL_VELOCITY, v.x, v.y, v.z);
}

// Value between 1 and 0 with 1 being unattenuated
void CAudioListener::setGain(float v) {
	gain = v;
	if (active_listener_id == listener_id)
		alListenerf(AL_GAIN, v);
}

void CAudioListener::setActive() const {
	if (listener_id != -1 && listener_id != active_listener_id) {
		active_listener_id = listener_id;

		alListener3f(AL_POSITION, position.x, position.y, position.z);
		alListener3f(AL_ORIENTATION, orientation.x, orientation.y, orientation.z);
		alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		alListenerf(AL_GAIN, gain);
	}
}