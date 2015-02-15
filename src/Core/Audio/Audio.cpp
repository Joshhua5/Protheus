#include "Audio.h"

using namespace Pro;
using namespace Audio;
using namespace Util;


CAudio::CAudio() { 
	device = alcOpenDevice(nullptr);
	if (device == nullptr)
		log.Report<LogCode::ERROR>("Unable to create context", __FUNCTION__, __LINE__);

	context = alcCreateContext(device, nullptr);
	if (context == nullptr)
		log.Report<LogCode::ERROR>("OpenAL Context Creation Error Code:" + alcGetError(device), __FUNCTION__, __LINE__);

	ALenum error_code;
	if ((error_code = alcGetError(device)) != ALC_NO_ERROR) {
		log.Report<LogCode::ERROR>("Unable to start OpenAL" + getErrorString(error_code), __FUNCTION__, __LINE__);
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
		log.Report<LogCode::ERROR>("Unable to create source: " + getErrorString(error_code), __FUNCTION__, __LINE__);
		source.id = 0;
		return source;
	}

	return source;
}

CAudioBuffer CAudio::loadAudio(AUDIO_FORMAT format, Buffer* file) { 
	CAudioBuffer buffer = 0;
	alGenBuffers(1, &buffer.id);
	// Load an audio file here
	if (file->size() == 0) {
		log.Report<LogCode::ERROR>("Empty buffer (CAudio::loadAudio)\0", __FUNCTION__, __LINE__);
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
		log.Report<LogCode::ERROR>("Unable to load audio file \0", __FUNCTION__, __LINE__);
		return 0;
	}

	alBufferData(buffer.id,
		(ALenum) track->format,
		track->stream[0],
		track->stream.size(),
		track->frequency);

	ALenum error_code = alGetError();
	if (error_code != AL_NO_ERROR) {
		log.Report<LogCode::ERROR>("Unable to load audio file: " + getErrorString(error_code), __FUNCTION__, __LINE__);
		delete track; 
		return 0;
	}

	delete track;
	return std::move(buffer);
}

std::string CAudio::getErrorString(ALenum num) {
	switch (num) {
	case AL_NO_ERROR: return std::string("There is no current error.");
	case AL_INVALID_NAME: return  std::string("Invalid name parameter.");
	case AL_INVALID_ENUM: return  std::string("Invalid parameter.");
	case AL_INVALID_VALUE: return  std::string("Invalid enum parameter value");
	case AL_INVALID_OPERATION: return  std::string("Illegal call.");
	case AL_OUT_OF_MEMORY: return  std::string("Unable to allocate memory.");
	default: return std::string("Unknown Error");
	} 
}