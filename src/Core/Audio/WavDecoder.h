/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
A Wavedecoder, which loads a WAVE file from a Buffer or CFile
and returns track.

SUPPORTED WAV FILES
UNSIGNED 8/16BIT MONO or STEREO 
-------------------------------------------------------------------------
History:
- 19:06:2014 Waring J.

*************************************************************************/
#pragma once
#include "IBitstreamDecoder.h" 

namespace Pro{
	namespace Audio{
		class CWavDecoder :
			public IBitstreamDecoder
		{
		public:  
			static CAudioData* load(CBuffer* buffer);
		};
	}
}
