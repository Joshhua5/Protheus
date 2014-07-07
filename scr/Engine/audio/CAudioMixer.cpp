
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

float inline getDropoff(Math::Vector2& pos){
	// Inverse Square Law  = P / 4 * PI * R * R
	// R : distance
	// P : Power 
	float distance = pos.hypotenuse();
	return 1.0f / (4.0f * PI * (distance * distance));
}

void CAudioMixer::process_stream(CAudioBuffer* stream, vector<CAudioSignal>* signals, atomic<bool>* ready){
	while (true){
		if (m_stream_refill.load()){
			// populate stream with silence
			switch (stream->channels){
			case 1: // Mono
				memset(stream->mono, 0, stream->mono->size);
				for each(auto signal in *signals)
					for (unsigned int x = 0; x < stream->mono->size; x++)
						static_cast<float*>(stream->mono->data)[x] += 
							static_cast<float*>(signal.stream.data)[x] * getDropoff(signal.position);
				break;
			case 2: // Sterio
				memset(stream->mono, 0, stream->mono->size);
				break;
			case 4: // 4.0
				memset(stream->mono, 0, stream->mono->size);
				break;
			case 6: // 5.1
				memset(stream->mono, 0, stream->mono->size);
				break;
			}
			 
			m_stream_ready.store(true);
			m_stream_refill.store(false);
		}
	}
}

CAudioBuffer* CAudioMixer::getStream(){
	return m_output_stream;
}

bool CAudioMixer::isStreamReady(){
	return m_stream_ready.load();
}
void CAudioMixer::refillStream(){
	m_stream_refill.store(true);
}

void CAudioMixer::playTrack(CAudioSignal){}

void CAudioMixer::playTrack(CAudioTrack){}

void CAudioMixer::pause(){}

void CAudioMixer::resume(){}

void CAudioMixer::stop(){}