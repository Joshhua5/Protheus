#pragma once
 
#include <string>
#include <atomic>
#include <Log.h>
#include "openal\alc.h"
#include "CAudioSource.h"
#include "CAudioBuffer.h"
#include "CAudioData.h"
#include "AudioListener.h"
#include "IBitstreamDecoder.h" 
#include "WavDecoder.h"

namespace Pro{
	namespace Audio{
		enum struct AUDIO_FORMAT {
			WAV,
			UNDEFINED
		};

		class CAudio{
			ALCdevice* device;
			ALCcontext* context;

			static  std::atomic<short> duplicateCount;

			std::string getErrorString(ALenum);
			  
		public:
			CAudio();
			~CAudio();
			 
			CAudioBuffer loadAudio(AUDIO_FORMAT format, Buffer* file);

			// Doesn't create a source after creating the file
			// returns the decoded audio file 

			CAudioSource createSource(const CAudioBuffer& buffer);
		}; 
	}
}  
