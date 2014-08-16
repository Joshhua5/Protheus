#include "CAudioMixer.h"

using namespace Pro;
using namespace Audio;

CAudioMixer::CAudioMixer() : CAudioMixer(2, 2048){}

CAudioMixer::CAudioMixer(char channels, unsigned short buffer_size){
	m_output_stream = new CAudioBuffer(channels, buffer_size);
	m_stream_active.store(false);
	m_stream_ready.store(false);
	m_stream_refill.store(false);
	m_stream_processor = thread(&process_stream, this);
} 

CAudioMixer::~CAudioMixer(){
	// Flag the thread to terminate
	m_stream_active.store(false); 
	// Wait for thread to terminate
	m_stream_processor.join();
	// Delete the m_output_stream
	delete m_output_stream;
}


float inline getDropoff(Math::Vector2& pos){
	// Inverse Square Law  = P / 4 * PI * R * R
	// R : distance
	// P : Power
	float distance = pos.length();
	return 1.0f / (4.0f * PI * (distance * distance));
}

void CAudioMixer::process_stream(){ 
	m_stream_active.store(true);
	while (m_stream_active.load()){
		if (m_stream_refill.load()){
			// populate stream with silence
			switch (m_output_stream->channels){
			case 1: // Mono
				memset(m_output_stream->mono, 0, m_output_stream->mono->size());
				for each(auto& signal in m_signals)
					for (unsigned x = 0; x < m_output_stream->mono->size(); ++x)
						m_output_stream->mono->data<float>()[x] +=
						signal.stream.data<float>()[x] * getDropoff(signal.position);
				break;
			case 2: // Sterio
				memset(m_output_stream->mono, 0, m_output_stream->mono->size());
				break;
			case 4: // 4.0
				memset(m_output_stream->mono, 0, m_output_stream->mono->size());
				break;
			case 6: // 5.1
				memset(m_output_stream->mono, 0, m_output_stream->mono->size());
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

void CAudioMixer::playTrack(const CAudioSignal&){}

void CAudioMixer::playTrack(const CAudioTrack&){}

void CAudioMixer::pause(){
	m_stream_active.store(false);
}

void CAudioMixer::resume(){
	m_stream_active.store(true);
}

void CAudioMixer::stop(){
	// Remove all signals from processing
	m_signals.erase(m_signals.begin(), m_signals.end());
	m_stream_active.store(false);
}