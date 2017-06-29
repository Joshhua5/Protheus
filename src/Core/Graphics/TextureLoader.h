#pragma once

#include <BufferReader.h>
#include <BufferWriter.h>
#include <Vector2.h> 
#include <Endian.h>
#include "Texture.h"
#include <PreDefinition.h>

namespace Pro {
	namespace Graphics {
		enum struct IMAGE_FORMAT {
			PNG,
			BMP,
			UNDEFINED
		}; 
		  
		struct PNGHeader {
			unsigned long width;
			unsigned long height;
		};

		struct BMPHeader {
			unsigned width;
			unsigned height;
			unsigned header_size;
			unsigned file_size;
			unsigned bmp_data_position;
			unsigned color_planes;
			unsigned bit_depth;
			// INFO
			unsigned compression_method;
			unsigned bmp_image_size;
			unsigned pixel_per_meter_hor;
			unsigned pixel_per_meter_ver;
			unsigned color_palette_count;
			unsigned important_colors;

			// V4Header
			unsigned        r_bitmask = 8;
			unsigned        g_bitmask = 8;
			unsigned        b_bitmask = 8;
			unsigned        a_bitmask = 8;
			unsigned        color_space_type;
			//CIEXYZTRIPLE	color_space_endpoints;
			unsigned        r_gamma = 1;
			unsigned        g_gamma = 1;
			unsigned        b_gamma = 1;

			// V5Header
			unsigned        bV5Intent;
			unsigned        bV5ProfileData;
			unsigned        bV5ProfileSize;

			// Extension

			unsigned pixel_size;

			void CorrectEndianess() {
				if (IS_BIG_ENDIAN) {
					bmp_image_size = bswap_32(bmp_image_size);
					header_size = bswap_32(header_size);
					color_planes = bswap_32(color_planes);
					width = bswap_32(width);
					height = bswap_32(height);
					bit_depth = bswap_32(bit_depth);
					compression_method = bswap_32(compression_method);
					bmp_image_size = bswap_32(bmp_image_size);
					pixel_per_meter_ver = bswap_32(pixel_per_meter_ver);
					pixel_per_meter_hor = bswap_32(pixel_per_meter_hor);
					color_palette_count = bswap_32(color_palette_count);
					important_colors = bswap_32(important_colors);
				}
			}
			std::string DebugString() const {
#ifdef PRO_DEBUG
				return
					"header_size " + std::to_string(header_size) +
					"\nfile_size " + std::to_string(file_size) +
					"\nbmp_data_position " + std::to_string(bmp_data_position) +
					"\nbmp_width " + std::to_string(width) +
					"\nbmp_height " + std::to_string(height) +
					"\ncolor_planes " + std::to_string(color_planes) +
					"\nbit_depth " + std::to_string(bit_depth) +
					"\ncompression_method " + std::to_string(compression_method) +
					"\nbmp_image_size " + std::to_string(bmp_image_size) +
					"\npixel_per_meter_hor " + std::to_string(pixel_per_meter_hor) +
					"\npixel_per_meter_ver " + std::to_string(pixel_per_meter_ver) +
					"\ncolor_palette_count " + std::to_string(color_palette_count) +
					"\nimportant_colors " + std::to_string(important_colors) +
					"\nr_bitmask " + std::to_string(r_bitmask) +
					"\ng_bitmask " + std::to_string(g_bitmask) +
					"\nb_bitmask " + std::to_string(b_bitmask) +
					"\na_bitmask " + std::to_string(a_bitmask) +
					"\ncolor_space_type " + std::to_string(color_space_type) +
					"\nr_gamma " + std::to_string(r_gamma) +
					"\ng_gamma " + std::to_string(g_gamma) +
					"\nb_gamma " + std::to_string(b_gamma) +
					"\nbV5Intent " + std::to_string(bV5Intent) +
					"\nbV5ProfileData " + std::to_string(bV5ProfileData) +
					"\nbV5ProfileSize " + std::to_string(bV5ProfileSize) +
					"\npixel_size " + std::to_string(pixel_size);
#else
				return "";
#endif
			}
		};


		class TextureLoader {
			static IMAGE_FORMAT QueryFormat(const Util::Buffer*); 
		public:
			static void EnableDebugging();
			static void DisableDebugging();
			//! Returns the raw texture data and corresponding header
			static Util::Buffer LoadBMPData(const Util::Buffer* file, BMPHeader& header);
			static Util::Buffer LoadPNGData(const Util::Buffer* file, PNGHeader& header);

			//! Loads a BMP from a file
            static std::shared_ptr<Texture> LoadBMP(const Util::Buffer* file);
			static std::shared_ptr<Texture> LoadPNG(const Util::Buffer* file);
			//! Accepts the raw data and a BMP header to load the texture from
			static std::shared_ptr<Texture> LoadTextureFromRaw(const Util::Buffer& data, const BMPHeader& header);
			static std::shared_ptr<Texture> LoadTextureFromRaw(const Util::Buffer& data, const PNGHeader& header);
			//! Loads a texture from a file
			static std::shared_ptr<Texture> LoadTexture(const Util::Buffer& file);
		};
	}
}