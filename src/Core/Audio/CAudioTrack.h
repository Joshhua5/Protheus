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
 
#include <CBuffer.h>
#include "openal\al.h"

namespace Pro{
	namespace Audio{
		struct CAudioTrack
		{
			CBuffer stream;
			 
			enum struct format_type{
				MONO8 = AL_FORMAT_MONO8,
				MONO16 = AL_FORMAT_MONO16,
				STEREO8 = AL_FORMAT_STEREO8,
				STEREO16 = AL_FORMAT_STEREO16
			}format; 

			// in Hertz
			ALsizei frequency; 
		};
	}
}
