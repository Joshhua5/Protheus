/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	The interface between the SDL Audio library and the produced audio buffers
-------------------------------------------------------------------------
History:
- 27:05:2014: Waring J.
*************************************************************************/
#pragma once
 
#include "CAudioMixer.h"
#include <SDL.h>

namespace Pro{
	namespace Audio{
		class CAudioDevice
		{ 
			//void audio_callback(CAudioMixer*, Uint8* stream, int length);
			unsigned int deviceID;
			CAudioMixer mixer;
			SDL_AudioSpec spec;
		public:
			CAudioDevice(SDL_AudioSpec);
			CAudioDevice();
			~CAudioDevice();

			CAudioMixer* getMixer();
			SDL_AudioStatus getDeviceState();
		}; 
	}
}

