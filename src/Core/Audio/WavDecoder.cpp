#include "WavDecoder.h"
#include <BufferReader.h>
#include <BufferWriter.h>

using namespace Pro;
using namespace Util;
using namespace Audio;

// First 44 bytes of the file are assosiated with the header

CAudioData* CWavDecoder::load(CBuffer* buffer){
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

	string chunkID = reader.read_string(4);
	unsigned chunkSize = reader.read<unsigned>();
	string format = reader.read_string(4);
	string subChunk1ID = reader.read_string(4);
	unsigned subChunk1Size = reader.read<unsigned>();
	short AudioFormat = reader.read<short>();
	short numChannels = reader.read<short>();
	unsigned sampleRate = reader.read<unsigned>();
	unsigned byteRate = reader.read<unsigned>();
	short blockAlign = reader.read<short>();
	short bitsPerSample = reader.read<short>();

	string subChunk2ID = reader.read_string(4);
	unsigned subChunk2Size = reader.read<unsigned>();

	// Check file

	 
	 
	if (chunkID != "RIFF" && format == "WAVE" && subChunk2ID == "data" && subChunk1ID == "fmt ") {
		error.reportErrorNR("Incorrect WAV format: " + chunkID + " fmt:" + format + " sc1:" + subChunk1ID + " sc2:" + subChunk2ID);
		return nullptr;
	} 

	// the size of the first subchunk, can change depending on what's stored
	// in the header 
	// 1 for PCM, anything else means there's some form on compression
	// and can't be read with this decoder 
	// subChunkSize2 indicated the size of the file after the header
	// can be used to determine how many samples are present in the file 

	CAudioData* out = new CAudioData;
	auto sample_count = ((subChunk2Size / (bitsPerSample / 8)) / numChannels);

	// Check for a supported format

	if (!(numChannels == 1 || numChannels == 2)){
		error.reportError("Unsupported WAV format: Invalid number of channels");
		delete out;
		return nullptr;
	}
	if (!(bitsPerSample == 8 || bitsPerSample == 16)){
		error.reportError("Unsupported WAV format: Invalid bits per sample");
		delete out;
		return nullptr;
	}

	// Start decoding the audio data  

	//unsigned int sampleSize = (headerData->BitsPerSample / 8) * headerData->channels; 
	
	//out->stream.init(sizeof(short) * numChannels * sample_count);

	Pro::Util::BufferWriter writer(&out->stream);

	switch (bitsPerSample){
	case 8: 
		out->stream.init(sizeof(char) * numChannels * sample_count);
		writer.write_elements<char>(reader.read_array<char>(sample_count * numChannels), sample_count * numChannels);
		out->format = (numChannels == 2) ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
		break;
	case 16:  
		out->stream.init(sizeof(short) * numChannels * sample_count);
		writer.write_elements<short>(reader.read_array<short>(sample_count * numChannels), sample_count * numChannels);
		out->format = (numChannels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		break;
	}
	out->frequency = sampleRate;

	return out;
}