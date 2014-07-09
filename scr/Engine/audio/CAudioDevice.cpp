#include "CAudioDevice.h"

using namespace Pro;
using namespace Audio;

void convertStream(CBuffer* buffer, CAudioDevice* dev, int channels, unsigned int inFreq){
	SDL_AudioCVT cvt;
	SDL_BuildAudioCVT(&cvt, AUDIO_F32, channels, inFreq, dev->getSpec().format, channels, dev->getSpec().freq);
	SDL_assert(cvt.needed);
	cvt.buf = static_cast<Uint8*>(buffer->data);
	cvt.len = buffer->size;
	SDL_ConvertAudio(&cvt);
}

void audio_callback(void* dev, Uint8* _stream, int length){
	auto device = static_cast<CAudioDevice*>(dev);
	auto mixer = device->getMixer();
	auto stream = mixer->getStream();

	if (mixer->isStreamReady()){
		// Mix the channels together
		switch (stream->channels){
		case 1:
			convertStream(stream->mono, device, 1, 48000);
			memmove(_stream, stream->mono->data, length);
			break;
		case 2:
			for (int x = 0; x < length / 2; ++x){
				SDL_AudioCVT convert;
				convert.src_format =
					_stream[x * 2] = static_cast<Uint8*>(stream->left->data)[x];
				_stream[(x * 2) + 1] = static_cast<Uint8*>(stream->right->data)[x];
			}
			break;
		case 4:
			break;
		case 6:
			break;
		}
	}
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
	_spec.callback = &audio_callback;
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

SDL_AudioSpec CAudioDevice::getSpec(){
	return spec;
}