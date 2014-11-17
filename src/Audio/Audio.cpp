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
	alcMakeContextCurrent(context);
}

CAudio::~CAudio(){ 
	alcDestroyContext(context);
	alcCloseDevice(device);
}


CAudioBuffer CAudio::loadAudio(IBitstreamDecoder* loader, CBuffer& file){
	CAudioBuffer buffer;
	alGenBuffers(1, &buffer.id);
	// Load an audio file here

	const auto track = loader->load(file);

	alBufferData(buffer.id,
		(ALenum) track->format,
		track->stream[0],
		track->stream.size(),
		track->frequency);

	delete track;
	return buffer;
}

CAudioSource CAudio::createSource(ALuint buffer){
	CAudioSource source;
	alGenSources(1, &source.id);
	alSourcef(source.id, AL_BUFFER, buffer);
	return source;
}