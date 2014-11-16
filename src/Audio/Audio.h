#pragma once

#include <al.h>
#include <string>
#include <Error.h>
#include <alc.h>
#include "CAudioSource.h"
#include "CAudioBuffer.h"
#include "CAudioTrack.h"
#include "AudioListener.h"
#include "IBitstreamDecoder.h"
 

namespace Pro{
	using namespace Audio;
	class Audio{
		ALCdevice* device;
		ALCcontext* context;
	public:
		CAudioListener listener;
	public:
		Audio() { 
			device = alcOpenDevice(nullptr);
			if (device == nullptr)
				error.reportErrorNR("Unable to create context");

			context = alcCreateContext(device, nullptr);
			if (context == nullptr)
				error.reportErrorNR("OpenAL Context Creation Error Code:" + alcGetError(device));
			alcMakeContextCurrent(context);
		}

		~Audio(){ 
		}

		CAudioBuffer loadAudio(IBitstreamDecoder* loader, CBuffer& file){
			CAudioBuffer buffer;
			alGenBuffers(1, &buffer.id);
			// Load an audio file here

			const auto track = loader->load(file); 

			alBufferData(buffer.id,
				(ALenum)track->format,
				track->stream[0],
				track->stream.size(),
				track->frequency);

			delete track;
			return buffer;
		}	
		CAudioSource createSource(ALuint buffer){
			CAudioSource source; 
			alGenSources(1, &source.id);
			alSourcef(source.id, AL_BUFFER, buffer); 
			return source; 
		}   
	};
}