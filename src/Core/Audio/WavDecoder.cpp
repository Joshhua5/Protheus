#include "WavDecoder.h"
#include <BufferReader.h>
#include <BufferWriter.h>

using namespace Pro; 
using namespace Audio;

// First 44 bytes of the file are assosiated with the header

CAudioData* CWavDecoder::load(Buffer* buffer){
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

	string chunkID = reader.ReadString(4);
	unsigned chunkSize = reader.Read<unsigned>();
	string format = reader.ReadString(4);
	string subChunk1ID = reader.ReadString(4);
	unsigned subChunk1Size = reader.Read<unsigned>();
	short AudioFormat = reader.Read<short>();
	short numChannels = reader.Read<short>();
	unsigned sampleRate = reader.Read<unsigned>();
	unsigned byteRate = reader.Read<unsigned>();
	short blockAlign = reader.Read<short>();
	short bitsPerSample = reader.Read<short>();

	string subChunk2ID = reader.ReadString(4);
	unsigned subChunk2Size = reader.Read<unsigned>();

	// Check file

	 
	 
	if (chunkID != "RIFF" && format == "WAVE" && subChunk2ID == "data" && subChunk1ID == "fmt ") {
		log.Report<LogCode::ERROR>("Incorrect WAV format: " + chunkID + " fmt:" + format + " sc1:" + subChunk1ID + " sc2:" + subChunk2ID, __FUNCTION__, __LINE__);
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
		log.Report<LogCode::ERROR>("Unsupported WAV format: Invalid number of channels", __FUNCTION__, __LINE__);
		delete out;
		return nullptr;
	}
	if (!(bitsPerSample == 8 || bitsPerSample == 16)){
		log.Report<LogCode::ERROR>("Unsupported WAV format: Invalid bits per sample", __FUNCTION__, __LINE__);
		delete out;
		return nullptr;
	}

	// Start decoding the audio data  

	//unsigned int sampleSize = (headerData->BitsPerSample / 8) * headerData->channels; 
	
	//out->stream.init(sizeof(short) * numChannels * sample_count);

	BufferWriter writer(&out->stream);

	switch (bitsPerSample){
	case 8: 
		out->stream.Init(sizeof(char) * numChannels * sample_count);
		writer.WriteElements<char>(reader.ReadArray<char>(sample_count * numChannels), sample_count * numChannels);
		out->format = (numChannels == 2) ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
		break;
	case 16:  
		out->stream.Init(sizeof(short) * numChannels * sample_count);
		writer.WriteElements<short>(reader.ReadArray<short>(sample_count * numChannels), sample_count * numChannels);
		out->format = (numChannels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		break;
	}
	out->frequency = sampleRate;

	return out;
}