#include "WavDecoder.h"
#include <BufferReader.h>
#include <BufferWriter.h>

using namespace Pro; 
using namespace Audio;

// First 44 bytes of the file are assosiated with the header

std::shared_ptr<CAudioData> CWavDecoder::load(Buffer* buffer){
	/*
	WAVE header
	0  : Chunk ID, contains "RIFF"
	4  : Chunk Size
	8  : Format, contains "WAVE"
	12 : Subchunk1ID, contains "fmt "
	16 : Subchunk1Size, contains the size of the subchunk
	20 : AudioFormat, PCM = 1
	22 : NumChannels
	24 : SampleRate
	28 : ByteRate
	32 : BlockAlign
	34 : BitsPerSample
	40 : ExtraData
	SubChunk1Size :
	Subchunk2ID, contains "data"
	SubChunk1Size + 4 :
	Subchunk2Size, conatins the size of the file, following
	The header
	SubChunk1Size + 8 :
	Data
	*/

	BufferReader reader(buffer);

	const string chunkID = reader.ReadString(4);
    // Check prematurely
    if(chunkID != "RIFF"){
        global_log.Report<LogCode::FAULT>("Incorrect WAV format, chunkID is not equal to RIFF", __FUNCTION__, __LINE__);
        return nullptr;
    }
    
	/*unsigned chunkSize = */reader.Read<unsigned>();
	const string format = reader.ReadString(4);
	const string subChunk1ID = reader.ReadString(4);
	/*unsigned subChunk1Size = */reader.Read<unsigned>();
	/*short AudioFormat = */reader.Read<short>();
	const short numChannels = reader.Read<short>();
	const unsigned sampleRate = reader.Read<unsigned>();
	/*unsigned byteRate = */ reader.Read<unsigned>();
	/*short blockAlign = */ reader.Read<short>();
	const short bitsPerSample = reader.Read<short>();

    // Lines commented out as variable isn't used,
    // but reading is still required to progress the reader
    
	const string subChunk2ID = reader.ReadString(4);
	const unsigned subChunk2Size = reader.Read<unsigned>();

	// Check file
 
	if (format == "WAVE" && subChunk2ID == "data" && subChunk1ID == "fmt ") {
		global_log.Report<LogCode::FAULT>("Incorrect WAV format: " + chunkID + " fmt:" + format + " sc1:" + subChunk1ID + " sc2:" + subChunk2ID, __FUNCTION__, __LINE__);
		return nullptr;
	} 

	// the size of the first subchunk, can change depending on what's stored
	// in the header 
	// 1 for PCM, anything else means there's some form on compression
	// and can't be read with this decoder 
	// subChunkSize2 indicated the size of the file after the header
	// can be used to determine how many samples are present in the file 
 
	// Check for a supported format

	if (!(numChannels == 1 || numChannels == 2)){
		global_log.Report<LogCode::FAULT>("Unsupported WAV format: Invalid number of channels", __FUNCTION__, __LINE__);
		return nullptr;
	}

	// Start decoding the audio data
    
    CAudioData* out;
    const unsigned sample_count = ((subChunk2Size / (bitsPerSample / 8)) / numChannels); 
    const unsigned sample_size = numChannels * sample_count;
    
	switch (bitsPerSample){
	case 8: {
		out = new CAudioData;
		BufferWriter writer(&out->stream);
		out->stream.Init(sizeof(char) * sample_size);
		writer.WriteElements<char>(reader.ReadArray<char>(sample_size), sample_size);
		out->format = (numChannels == 2) ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
		break;
	}
	case 16: {
		out = new CAudioData;
		BufferWriter writer(&out->stream);
		out->stream.Init(sizeof(short) * sample_size);
		writer.WriteElements<short>(reader.ReadArray<short>(sample_size), sample_size);
		out->format = (numChannels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		break;
	}
    default:
            global_log.Report<LogCode::FAULT>("Unsupported WAV format: Invalid bits per sample", __FUNCTION__, __LINE__);
            return nullptr;
	}
    
	out->frequency = sampleRate;
    return std::shared_ptr<CAudioData>(out);
}