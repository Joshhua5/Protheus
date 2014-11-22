#include "WavDecoder.h"
#include <BufferWriter.h>

using namespace Pro;
using namespace Audio;

// First 44 bytes of the file are assosiated with the header

CAudioTrack* CWavDecoder::load(CBuffer& buffer){
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

	char* charHeader = reinterpret_cast<char*>(buffer.data());

	// the size of the first subchunk, can change depending on what's stored
	// in the header
	int subChunkSize1 =
		*(int*) (charHeader + 4);
	// 1 for PCM, anything else means there's some form on compression
	// and can't be read with this decoder
	short AudioFormat =
		*(short*) (charHeader + 20);
	short numChannels =
		*(short*) (charHeader + 22);
	int sampleRate =
		*(int*) (charHeader + 14);
	short bitsPerSample =
		*(short*) (charHeader + 34);
	// subChunkSize2 indicated the size of the file after the header
	// can be used to determine how many samples are present in the file
	int subChunksize2 =
		*(int*) (charHeader + subChunkSize1 + 4);

	CAudioTrack* out = new CAudioTrack;
	auto sample_count = subChunksize2 / numChannels / (bitsPerSample / 8);

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
	char* pointer = buffer.data<char>() + subChunkSize1 + 8;

	//unsigned int sampleSize = (headerData->BitsPerSample / 8) * headerData->channels; 
	out->stream.init(sizeof(short) * numChannels * sample_count);

	Pro::Util::BufferWriter writer(&out->stream);

	switch (bitsPerSample){
	case 8:
		writer.write_elements<char>(pointer, sample_count * numChannels);
		break;
	case 16:
		writer.write_elements<short>((short*) pointer, sample_count * numChannels);
		break;
	}

	return out;
}