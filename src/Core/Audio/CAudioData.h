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
 
#include <Buffer.h>
#include "openal\al.h"

namespace Pro{
	namespace Audio{
		using namespace Util;
		struct CAudioData
		{
			Buffer stream;
			 
			ALenum format; 
			// in Hertz
			ALsizei frequency; 
		};
	}
}
