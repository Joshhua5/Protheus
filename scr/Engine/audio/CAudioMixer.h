/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	A class with the functionality of compining multiple tracks and their
	effects to produce the output stream for the audio device

-------------------------------------------------------------------------
History:
- 09:06:2014 Waring J.
*************************************************************************/
#pragma once 


#include "CAudioSignal.h"

namespace Pro{
	namespace Audio{
		class CAudioMixer
		{
			Uint8* output_stream;
			std::vector<CAudioSignal> signals;
		public:
			CAudioMixer();
			~CAudioMixer();
			 
			void playTrack(CAudioSignal);
			// Will create a signal from the track
			void playTrack(CAudioTrack);
			void pause();
			void resume();
			void stop();
		};
	}
}


