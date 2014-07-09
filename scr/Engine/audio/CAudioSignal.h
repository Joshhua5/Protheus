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
#include "..\Math.h"

namespace Pro{
	namespace Audio{
		class CAudioSignal
		{
		public:
			CBuffer stream;
			float power;
			unsigned char channels;
			unsigned int format;
			unsigned int frequency;
			unsigned int sample_rate;
			Math::Vector2 position;
			std::vector<IAudioEffect> effects;
		public:
			CAudioSignal(Math::Vector2, CAudioTrack);
			CAudioSignal();
			~CAudioSignal();

			void addEffect(IAudioEffect);

			static inline string lGetMetatable(){
				return "audio_signal_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				//fields.push_back({ "addEffect", &T::lAddEffect });
			}
		};
	}
}