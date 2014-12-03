#include "Audio.h"

using namespace Pro;
using namespace Audio;


CAudio::CAudio() { 
	device = alcOpenDevice(nullptr);
	if (device == nullptr)
		error.reportErrorNR("Unable to create context");

	context = alcCreateContext(device, nullptr);
	if (context == nullptr)
		error.reportErrorNR("OpenAL Context Creation Error Code:" + alcGetError(device));

	ALenum error_code;
	if ((error_code = alcGetError(device)) != ALC_NO_ERROR) {
		error.reportError("Unable to start OpenAL" + getErrorString(error_code));
		return;
	} 

	alcMakeContextCurrent(context);

	alDistanceModel(AL_LINEAR_DISTANCE);
}

CAudio::~CAudio() {
	alcDestroyContext(context);
	alcCloseDevice(device);
}

CAudioSource CAudio::createSource(const CAudioBuffer& buffer) {
	ALuint id = 0;
	alGenSources(1, &id); 
	CAudioSource source(id);
	alSourcei(source.id, AL_BUFFER, static_cast<ALint>(buffer.id));

	ALenum error_code;
	if ((error_code = alcGetError(device)) != ALC_NO_ERROR) {
		error.reportError("Unable to create source: " + getErrorString(error_code));
		source.id = 0;
		return source;
	}

	return source;
}

CAudioBuffer CAudio::loadAudio(AUDIO_FORMAT format, CBuffer* file) { 
	CAudioBuffer buffer = 0;
	alGenBuffers(1, &buffer.id);
	// Load an audio file here
	if (file->size() == 0) {
		error.reportErrorNR("Empty buffer (CAudio::loadAudio)\0");
		alDeleteBuffers(1, &buffer.id);
		return 0;
	}

	const CAudioData* track = nullptr;

	switch (format) {
	case AUDIO_FORMAT::WAV:
		track = CWavDecoder::load(file);
		break;
	}

	if (track == nullptr) {
		error.reportErrorNR("Unable to load audio file \0");
		return 0;
	}

	alBufferData(buffer.id,
		(ALenum) track->format,
		track->stream[0],
		track->stream.size(),
		track->frequency);

	ALenum error_code = alGetError();
	if (error_code != AL_NO_ERROR) {
		error.reportError("Unable to load audio file: " + getErrorString(error_code));
		delete track; 
		return 0;
	}

	delete track;
	return std::move(buffer);
}

string CAudio::getErrorString(ALenum num) {
	switch (num) {
	case AL_NO_ERROR: return string("There is no current error.");
	case AL_INVALID_NAME: return string("Invalid name parameter.");
	case AL_INVALID_ENUM: return string("Invalid parameter.");
	case AL_INVALID_VALUE: return string("Invalid enum parameter value");
	case AL_INVALID_OPERATION: return string("Illegal call.");
	case AL_OUT_OF_MEMORY: return string("Unable to allocate memory.");
	default: return string("Unknown Error");
	} 
}