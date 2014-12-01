/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	A structure to pass Buffers between threads, each buffer contains a channel
	If Channels is 1 then we only use the mono buffer
	If Channels is 2 then we use left and right
	If Channels is 4 then we use front_left, front_right, back_left, back_right
	If Channels is 6 then we use  front_left, front_right, back_left, back_right,
	Center and Low_frew
-------------------------------------------------------------------------
History:
- 27:05:2014: Waring J.
*************************************************************************/
#pragma once

#include "openal\al.h"

namespace Pro{
	namespace Audio{ 
		struct CAudioBuffer
		{
			ALuint id;

			~CAudioBuffer(){
				alDeleteBuffers(1, &id);
			}
		};
	}
}
