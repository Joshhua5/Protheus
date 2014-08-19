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
			CAudioBuffer* m_output_stream;
			atomic<bool> m_stream_ready;
			atomic<bool> m_stream_refill;
			atomic<bool> m_stream_active;
			vector<CAudioSignal> m_signals;
			void process_stream();
			thread m_stream_processor;
		public:
			// Defaults to 2 channels with a 2KB buffer
			CAudioMixer();
			CAudioMixer(char channels, unsigned short buffer_size);
			CAudioMixer(const CAudioMixer& copy) = delete;
			// Moving or copying will break the internal
			// thread reference.
			CAudioMixer(CAudioMixer&& move) = delete;
			~CAudioMixer();

			CAudioBuffer* getStream();

			// returns true if the stream is ready to be
			// copied into the device
			bool isStreamReady();

			// inform the process to start processing
			// a new segment of the stream
			void refillStream();

			// Add a signal to the output stream.
			void playTrack(const CAudioSignal&);

			// Will create a signal from the track
			// and call playTrack with the new signal
			void playTrack(const CAudioTrack&);

			// Pause the processing
			void pause();

			// Resume processing of the audio stream
			void resume();

			// Stop all processing of the audio stream
			// and reset all active signals
			void stop();

			// LUA Functions

			/*constexpr*/ static const char* lGetMetatable(){
				return "audio_mixer_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				//fields.push_back({ "update", &T::lUpdate });
			}
		};
	}
}
