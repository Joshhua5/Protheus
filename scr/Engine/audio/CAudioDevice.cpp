
#include "CAudioDevice.h"

using namespace Pro;
using namespace Audio;


void audio_callback(void* dev, Uint8* stream, int length){
	auto device = static_cast<CAudioDevice*>(dev);
	auto mixer = device->getMixer();
	if (mixer->isStreamReady())
		memmove(stream, mixer->getStream()->data , length);
}

CAudioDevice::CAudioDevice(SDL_AudioSpec _spec){
	_spec.callback = audio_callback;
	_spec.userdata = this;
	if (SDL_OpenAudio(&_spec, &spec) < 0){
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError()); 
	}
}

CAudioDevice::CAudioDevice(){
	SDL_AudioSpec _spec;
	_spec.channels = 2;
	_spec.format = AUDIO_S16;
	_spec.freq = 44100;
	_spec.samples = 1024;
	_spec.callback = audio_callback;
	_spec.userdata = this;

	if (SDL_OpenAudio(&_spec, &spec) < 0){
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		CAudioDevice();
	}

}
CAudioDevice::~CAudioDevice(){ 
	SDL_CloseAudioDevice(deviceID);
}

CAudioMixer* CAudioDevice::getMixer(){
	return &mixer;
}

SDL_AudioStatus CAudioDevice::getDeviceState(){
	return SDL_GetAudioStatus();
}

