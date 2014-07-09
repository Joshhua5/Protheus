/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Structure containing information about a audio track,
when a audio track is to be played, the data inside the stream
is copied and becomes a audio signal, meaning the audio track is a
copy template and never used directly for processing.

-------------------------------------------------------------------------
History:
- 09:06:2014 Waring J.
*************************************************************************/
#pragma once

#include "..\util\CBuffer.h"

namespace Pro{
	namespace Audio{
		struct CAudioTrack
		{
			CBuffer stream;

			char channels;

			unsigned int chunkSize;
			unsigned int BitsPerSample;
			unsigned int sample_rate;
			unsigned int sample_count;

			// reserved for decoding
			unsigned int audioFormat;
			unsigned int headerSize;
		};
	}
}
