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
#include "IBitstreamDecoder.h"
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

			// Returns the details of the Audio Device which 
			SDL_AudioSpec getSpec();
			// Returns the mixer instance responsible for
			// processing of Audio to be played
			CAudioMixer* getMixer();
			// Returns the device state (Playing, Paused, Stopped)
			SDL_AudioStatus getDeviceState();

			// Lua Functions

			/*constexpr*/ static const char* lGetMetatable(){
				return "audio_device_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				//fields.push_back({ "update", &T::lUpdate });
			}
		};
	}
}
