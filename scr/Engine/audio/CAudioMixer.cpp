
#include "CAudioMixer.h"

using namespace Pro;
using namespace Audio;

CAudioMixer::CAudioMixer()
{
}


CAudioMixer::~CAudioMixer()
{
}

/**
*
*
*
*
*
**/

void CAudioMixer::process_stream(CAudioBuffer* stream, vector<CAudioSignal>* signals, atomic<bool>* ready){
	while (true){
		if (stream_refill.load()){
			// populate stream with silence
			switch (stream->channels){
			case 1:
				memset(stream->mono, 0, stream->mono->size);
			}
			for each(auto signal in *signals){
				// Signal Processing
				switch (signal.channels){
				case 1: // mono
					for (unsigned int x = 0; x < stream->mono->size; x++)

					break;
				case 2: // sterio
					break;
				}
			}
			stream_ready.store(true);
			stream_refill.store(false);
		}
	}
}

CAudioBuffer* CAudioMixer::getStream(){
	return output_stream;
}

bool CAudioMixer::isStreamReady(){
	return stream_ready.load();
}
void CAudioMixer::refillStream(){
	stream_refill.store(true);
}

void CAudioMixer::playTrack(CAudioSignal){}

void CAudioMixer::playTrack(CAudioTrack){}

void CAudioMixer::pause(){}

void CAudioMixer::resume(){}

void CAudioMixer::stop(){}