/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
A Wavedecoder, which loads a WAVE file from a Buffer or CFile
and returns track.
-------------------------------------------------------------------------
History:
- 19:06:2014 Waring J.

*************************************************************************/
#pragma once
#include "IBitstreamDecoder.h"
#include <SDL.h>

namespace Pro{
	namespace Audio{
		class CWavDecoder :
			public IBitstreamDecoder
		{
		public:
			CWavDecoder();
			~CWavDecoder();

			CAudioTrack* load(Util::CFile& file);
			CAudioTrack* load(CBuffer& buffer);
		};
	}
}
