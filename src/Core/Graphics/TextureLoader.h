#pragma once

#include <CBuffer.h>
#include <BufferReader.h>
#include <BufferWriter.h>
#include <Vector2.h>
#include "Texture.h"

namespace Pro {
	enum struct IMAGE_FORMAT {
		PNG,
		BMP,
		UNDEFINED
	};

	enum struct BMP_COMRESSION {
		_BI_RGB = 0,
		_BI_RLE8 = 1,
		_BI_RLE4 = 2,
		_BI_BITFIELDS = 3,
		_BI_JPEG = 4,
		_BI_PNG = 5,
		_BI_ALPHABITFIELDS = 6,
		_BI_CMYK = 11,
		_BI_CMYKRLE8 = 12,
		_BI_CMYKTLE4 = 13
	};

	struct ColorDefinition {
		GLubyte r, g, b, a;
	}; 

	class TextureLoader { 
		static IMAGE_FORMAT queryFormat(CBuffer*);
	public:

		static Texture* loadBMP(CBuffer*);
		static Texture* loadTexture(CBuffer*);
	};
}
