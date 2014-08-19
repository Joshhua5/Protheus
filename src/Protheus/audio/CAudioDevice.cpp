#include "CAudioDevice.h"

using namespace Pro;
using namespace Audio;

void inline convertStream(CBuffer* buffer, CAudioDevice* dev, int channels, unsigned int inFreq){
	SDL_AudioCVT cvt;
	SDL_BuildAudioCVT(&cvt, AUDIO_F32, channels, inFreq, dev->getSpec().format, channels, dev->getSpec().freq);
	SDL_assert(cvt.needed);

	cvt.buf = buffer->data<Uint8>();
	cvt.len = buffer->size();
	SDL_ConvertAudio(&cvt);
}

void audio_callback(void* dev, Uint8* _stream, int length){
	const auto device = static_cast<CAudioDevice*>(dev);
	const auto mixer = device->getMixer();
#ifdef DEBUG
	if (mixer == nullptr){
		error.reportError("Mixer is a nullptr, but the audio_callback is being executed");
		return;
	}
#endif
	const auto stream = mixer->getStream(); 

	// Populate the audio stream if the mixer has processed
	// enough data
	if (mixer->isStreamReady()){
		switch (stream->channels){
		case 1:
			// convert the stream into the devices format.
			// Consider having the stream pre converted in the mixer
			convertStream(stream->mono, device, 1, 48000);
			// Move the stream into the device
			memmove(_stream, stream->mono->data(), length);
			break;
		case 2:
			/*for (int x = 0; x < length / 2; ++x){
				_stream[x * 2] = stream->left->data<Uint8>()[x];
				_stream[(x * 2) + 1] = stream->right->data<Uint8>()[x];
				}*/
			break;
		case 4:
			break;
		case 6:
			break;
		}
	}
}

CAudioDevice::CAudioDevice(){
	// Create a request of the settings of the device,
	// What we request may not be approved in which case
	// we create the device with what was approved.
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
	// Create mixer
	mixer = new CAudioMixer(spec.channels, 2048);
}


CAudioDevice::CAudioDevice(CAudioDevice&& move){
	spec = std::move(move.spec);
	mixer = move.mixer;
	move.mixer = nullptr;
	deviceID = move.deviceID;
	move.deviceID = NULL;
}

CAudioDevice::CAudioDevice(SDL_AudioSpec _spec){
	_spec.callback = audio_callback;
	// Allow the callback to access this CAudioDevice instance.
	_spec.userdata = this;
	if (SDL_OpenAudio(&_spec, &spec) < 0){
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
	}
	// Create mixer
	mixer = new CAudioMixer(spec.channels, 2048);
}

CAudioDevice::~CAudioDevice(){
	SDL_CloseAudioDevice(deviceID);
	delete mixer;
}
 
CAudioMixer* CAudioDevice::getMixer(){
	return mixer;
}

SDL_AudioStatus CAudioDevice::getDeviceState(){
	return SDL_GetAudioStatus();
}

SDL_AudioSpec CAudioDevice::getSpec(){
	return spec;
}