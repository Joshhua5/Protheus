#include "Audio.h"

using namespace Pro;
using namespace Audio;
using namespace Util;

CAudio::CAudio() {
	device = alcOpenDevice(nullptr);
	if (device == nullptr)
        global_log.Report<LogCode::FAULT>("Unable to create context: " + std::to_string(alcGetError(device)), __FUNCTION__, __LINE__);

	context = alcCreateContext(device, nullptr);
	if (context == nullptr)
        global_log.Report<LogCode::FAULT>("OpenAL Context Creation Error Code: " + std::to_string(alcGetError(device)), __FUNCTION__, __LINE__);

	ALenum error_code;
	if ((error_code = alcGetError(device)) != ALC_NO_ERROR) {
		global_log.Report<LogCode::FAULT>("Unable to start OpenAL" + getErrorString(error_code), __FUNCTION__, __LINE__);
        alcDestroyContext(context);
        alcCloseDevice(device);
        device = nullptr;
        context = nullptr;
		return;
	}

	alcMakeContextCurrent(context);

	alDistanceModel(AL_LINEAR_DISTANCE);
}

CAudio::~CAudio() {
	alcDestroyContext(context);
	alcCloseDevice(device);
}

bool CAudio::HasError() const {
    if(device == nullptr)
        return true;
    ALCenum error = alcGetError(device);
    ALenum erroral = alGetError();
    if(error != ALC_NO_ERROR || erroral != AL_NO_ERROR){
        global_log.Report<LogCode::AUDIO>("OpenAL Has Error ALC:" + std::to_string(error) +
                                          " AL: " + getErrorString(erroral), __FUNCTION__, __LINE__);
        return true;
    }
    if(context == nullptr)
        return true;
    return false;
}

CAudioSource CAudio::createSource(const CAudioBuffer& buffer) {
	ALuint id;
	alGenSources(1, &id);
	CAudioSource source(id);
	alSourcei(source.id, AL_BUFFER, static_cast<ALint>(buffer.id));

	ALenum error_code;
	if ((error_code = alcGetError(device)) != ALC_NO_ERROR) {
		global_log.Report<LogCode::FAULT>("Unable to create source: " + getErrorString(error_code), __FUNCTION__, __LINE__);
		source.id = 0;
		return source;
	}

	return source;
}

CAudioBuffer CAudio::loadAudio(AUDIO_FORMAT format, Buffer* file) {
	CAudioBuffer buffer;
	alGenBuffers(1, &buffer.id);
	// Load an audio file here
	if (file->size() == 0) {
		global_log.Report<LogCode::FAULT>("Empty buffer (CAudio::loadAudio)\0", __FUNCTION__, __LINE__);
		alDeleteBuffers(1, &buffer.id);
		return 0;
	}

    std::shared_ptr<CAudioData> track;

	switch (format) {
        case AUDIO_FORMAT::WAV:
            track = CWavDecoder::load(file);
            break;
        case AUDIO_FORMAT::UNDEFINED:
            global_log.Report<LogCode::FAULT>("Unsupported Audio Format\0", __FUNCTION__, __LINE__);
            return 0;
	}

	if (track == nullptr) {
		global_log.Report<LogCode::FAULT>("Unable to load audio file\0", __FUNCTION__, __LINE__);
		return 0;
	}

	alBufferData(buffer.id,
		(ALenum) track->format,
		track->stream[0],
		(int)track->stream.size(),
		track->frequency);

	ALenum error_code = alGetError();
	if (error_code != AL_NO_ERROR) {
		global_log.Report<LogCode::FAULT>("Unable to load audio file: " + getErrorString(error_code), __FUNCTION__, __LINE__);
		return 0;
	}

	return std::move(buffer);
}

std::string CAudio::getErrorString(ALenum num) const {
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
