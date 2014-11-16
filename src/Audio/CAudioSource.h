/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
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
#include <vector>
#include <al.h>

namespace Pro{
	namespace Audio{
		class CAudioSource
		{
		public: 
			CAudioSource();
			~CAudioSource();

			ALuint id;
			std::vector<IAudioEffect> effects;  
			void addEffect(IAudioEffect);
	
			void play(){
				alSourcePlay(id);
			}
		};
	}
}