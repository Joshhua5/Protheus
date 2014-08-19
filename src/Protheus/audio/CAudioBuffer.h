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

#include "..\util\CBuffer.h"

namespace Pro{
	namespace Audio{ 
		struct CAudioBuffer
		{
			char channels;
			union{
				CBuffer* mono = nullptr;
				CBuffer* left;
				CBuffer* front_left;
			};
			union{
				CBuffer* right = nullptr;
				CBuffer* front_right;
			};
			CBuffer* back_left = nullptr;
			CBuffer* back_right = nullptr;
			CBuffer* center = nullptr;
			CBuffer* low_freq = nullptr;

			// Initializer of a CAudioBuffer 
			CAudioBuffer(char channels, unsigned short buffer_size){
				this->channels = channels;
				switch (channels){
				case 1:
					mono = new CBuffer(buffer_size); 
					return;
				case 2:
					left = new CBuffer(buffer_size);
					right = new CBuffer(buffer_size);
					return;
				case 4:
					front_left = new CBuffer(buffer_size);
					front_right = new CBuffer(buffer_size);
					back_left = new CBuffer(buffer_size);
					back_right = new CBuffer(buffer_size);
					return;
				case 6:
					front_left = new CBuffer(buffer_size);
					front_right = new CBuffer(buffer_size);
					center = new CBuffer(buffer_size);
					back_left = new CBuffer(buffer_size);
					back_right = new CBuffer(buffer_size);
					low_freq = new CBuffer(buffer_size);
					return;
				}
			}

			~CAudioBuffer(){
				delete mono;
				delete right;
				delete back_left;
				delete back_right;
				delete center;
				delete low_freq;
			}
		};
	}
}
