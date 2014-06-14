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

#include <atomic>
#include <thread>
#include "CAudioBuffer.h"
#include "CAudioSignal.h"

namespace Pro{
	namespace Audio{
		using namespace std;

		class CAudioMixer
		{
			CAudioBuffer* output_stream; 
			atomic<bool> stream_ready;
			atomic<bool> stream_refill;
			vector<CAudioSignal> signals; 
			void process_stream(CAudioBuffer* stream, vector<CAudioSignal>* signals, atomic<bool>* ready);
			thread stream_processor;
		public:
			CAudioMixer();
			~CAudioMixer();
			CAudioBuffer* getStream();
			bool isStreamReady();
			void refillStream();
			void playTrack(CAudioSignal);
			// Will create a signal from the track
			void playTrack(CAudioTrack);
			void pause();
			void resume();
			void stop();
		};
	}
}


