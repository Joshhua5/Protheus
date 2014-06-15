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

