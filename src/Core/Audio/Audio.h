#pragma once
 
#include <string>
#include <atomic>
#include <Error.h>
#include "openal\alc.h"
#include "CAudioSource.h"
#include "CAudioBuffer.h"
#include "CAudioTrack.h"
#include "AudioListener.h"
#include "IBitstreamDecoder.h"
 

namespace Pro{
	namespace Audio{
		class CAudio{
			ALCdevice* device;
			ALCcontext* context;

			static atomic<short> duplicateCount; 
			CAudioListener listener;
		public:
			CAudio();
			~CAudio();
			
			CAudioBuffer loadAudio(IBitstreamDecoder* loader, CBuffer& file); 
			CAudioSource createSource(ALuint buffer);
		};
	}
}