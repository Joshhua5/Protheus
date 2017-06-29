/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:
A playable and mutable copy of a audio track, effects can be applied
to the signal.

-------------------------------------------------------------------------
History:
- 09:06:2014 Waring J.
*************************************************************************/
#pragma once

#include "IAudioEffect.h"
#include <Vector3.h>
#include <vector> 

namespace Pro{
	namespace Audio{ 
		using namespace Math;
		class CAudioSource
		{
		public: 
			CAudioSource(ALuint id);
			~CAudioSource();

			ALuint id;
            
            void play(){
				alSourcePlay(id);
			}

			void position(const Vector3<float>& v){ alSource3f(id, AL_POSITION, v.x, v.y, v.z); }
			void setOrientation(const Vector3<float>& v){ alSource3f(id, AL_ORIENTATION, v.x, v.y, v.z); }
			void setVelocity(const Vector3<float>& v) { alSource3f(id, AL_VELOCITY, v.x, v.y, v.z); }
		};
	}
}