#include "WavDecoder.h"

using namespace Pro;
using namespace Audio;

// First 44 bytes of the file are assosiated with the header

CAudioTrack* decodeHeader(CBuffer& header){
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

	char* charHeader = static_cast<char*>(header.data);

	// the size of the first subchunk, can change depending on what's stored
	// in the header
	int subChunkSize1 =
		*(int*)(charHeader + 4);
	// 1 for PCM, anything else means there's some form on compression
	// and can't be read with this decoder
	short AudioFormat =
		*(short*)(charHeader + 20);
	short numChannels =
		*(short*)(charHeader + 22);
	int sampleRate =
		*(int*)(charHeader + 14);
	short bitsPerSample =
		*(short*)(charHeader + 34);
	// subChunkSize2 indicated the size of the file after the header
	// can be used to determine how many samples are present in the file
	int subChunksize2 =
		*(int*)(charHeader + subChunkSize1 + 4);

	CAudioTrack* out = new CAudioTrack;
	out->channels = static_cast<char>(numChannels);
	out->sample_rate = sampleRate;
	out->BitsPerSample = bitsPerSample;
	out->audioFormat = AudioFormat;
	out->sample_count = subChunksize2 / numChannels / (bitsPerSample / 8);
	// + 8 for the size of the second subchunk
	out->headerSize = subChunkSize1 + 8;
	return out;
}

// uses the headerData to read the samples
// this will convert the data to AUDIO_F32
// as this is the format internally used
CBuffer decodeData(CAudioTrack* headerData, CBuffer& data){
	char* pointer = static_cast<char*>(data.data) + headerData->headerSize;
	//unsigned int sampleSize = (headerData->BitsPerSample / 8) * headerData->channels;
	unsigned int outSampleSize = headerData->channels * sizeof(float);
	CBuffer out(headerData->sample_count * headerData->channels * sizeof(float));

	for (unsigned sample = 0; sample < headerData->sample_count; ++sample){
		for (int channel = 0; channel < headerData->channels; channel++){
			switch (headerData->BitsPerSample){
			case 8:
				static_cast<float*>(out.data)[sample * outSampleSize + channel] =
					static_cast<char>(*pointer);
				pointer += sizeof(char);
				break;
			case 16:
				static_cast<float*>(out.data)[sample * outSampleSize + channel] =
					static_cast<short>(*pointer);
				pointer += sizeof(short);
				break;
			case 24:
				int value;
				memcpy(&value, pointer, 3);
				static_cast<float*>(out.data)[sample * outSampleSize + channel] =
					static_cast<float>(value);
				pointer += (sizeof(char) * 3);
				break;
			case 32:
				static_cast<float*>(out.data)[sample * outSampleSize + channel] =
					static_cast<float>(*pointer);
				pointer += sizeof(float);
				break;
			}
		}
	}
	return out;
}

CAudioTrack* CWavDecoder::load(IO::CFile& file){
	return load(file.read(file.getSize()));
}

CAudioTrack* CWavDecoder::load(CBuffer& buffer){
	CAudioTrack* track = decodeHeader(buffer);
	track->stream = decodeData(track, buffer);
	track->BitsPerSample = sizeof(float);
	return track;
}