#include "TextureLoader.h"

using namespace Pro;
using namespace Util;
using namespace Graphics;
using namespace Math;

inline GLuint toColor(unsigned r_bitmask, unsigned g_bitmask, unsigned b_bitmask, unsigned a_bitmask, ColorDefinition def) {
	// TODO convert bitmask's into 8 bits per channel
	// incase a image comes with weird bitmasks
	GLuint out = 0;
	//out |= (def.r << (8 - r_bitmask));
	//out |= 0xF0 & (def.g << (8 - g_bitmask));
	//out |= 0xF00 & (def.b << (8 - b_bitmask));
	//out |= 0xF000 & (def.a << (8 - a_bitmask));
	out |= def.r;
	out |= def.g << 8;
	out |= def.b << 16;
	out |= def.a << 24;

	return out;
}
namespace Pro {
	namespace Graphics { 
		struct BMPHeader {
			unsigned header_size;
			unsigned long bmp_width;
			unsigned long bmp_height;
			unsigned color_planes;
			unsigned bit_depth;
			// INFO
			unsigned compression_method;
			unsigned bmp_image_size;
			unsigned long pixel_per_meter_hor;
			unsigned long pixel_per_meter_ver;
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
		};
	}
}

inline void decodeBitmapHeader(BMPHeader& header, BufferReader& reader) {
	switch (header.header_size) {
	case 52: // BITMAPv2INFOHEADER 
	case 56: // BITMAPv3INFOHEADER
		header.bmp_width = reader.Read<unsigned long>();
		header.bmp_height = reader.Read<unsigned long>();
		header.color_planes = reader.Read<unsigned short>();
		header.bit_depth = reader.Read<unsigned short>();
		header.compression_method = reader.Read<unsigned>();
		header.bmp_image_size = reader.Read<unsigned>();
		header.pixel_per_meter_hor = reader.Read<long>();
		header.pixel_per_meter_ver = reader.Read<long>();
		header.color_palette_count = reader.Read<unsigned>();
		header.important_colors = reader.Read<unsigned>();
		header.r_bitmask = reader.Read<unsigned>();
		header.g_bitmask = reader.Read<unsigned>();
		header.b_bitmask = reader.Read<unsigned>();
		if (header.header_size == 56)
			header.a_bitmask = reader.Read<unsigned>();
		break;
	case 64: // OS22XBITMAPHEADER 
		break;
	default:
		if (header.header_size == 12) {
			header.bmp_width = reader.Read<unsigned short>();
			header.bmp_height = reader.Read<unsigned short>();
			header.color_planes = reader.Read<unsigned short>();
			header.bit_depth = reader.Read<unsigned short>();
		}
		// BITMAPINFOHEADER
		if (header.header_size == 40 || header.header_size == 108 || header.header_size == 124) {
			header.bmp_width = reader.Read<int>();
			header.bmp_height = reader.Read<int>();
			header.color_planes = reader.Read<unsigned short>();
			header.bit_depth = reader.Read<unsigned short>();
			header.compression_method = reader.Read<unsigned>();
			header.bmp_image_size = reader.Read<unsigned>();
			header.pixel_per_meter_hor = reader.Read<int>();
			header.pixel_per_meter_ver = reader.Read<int>();
			header.color_palette_count = reader.Read<unsigned>();
			header.important_colors = reader.Read<unsigned>();
		}
		// BITMAPV4HEADER   
		if (header.header_size == 108 || header.header_size == 124) {
			header.r_bitmask = reader.Read<unsigned>();
			header.g_bitmask = reader.Read<unsigned>();
			header.b_bitmask = reader.Read<unsigned>();
			header.a_bitmask = reader.Read<unsigned>();
			header.color_space_type = reader.Read<unsigned>();
		}
		// BITMAPV5HEADER  
		if (header.header_size == 124) {
			header.bV5Intent = reader.Read<unsigned>();
			header.bV5ProfileData = reader.Read<unsigned>();
			header.bV5ProfileSize = reader.Read<unsigned>();
		}
	}
}


IMAGE_FORMAT TextureLoader::QueryFormat(Buffer* buffer) {
	// check for BMP
	BufferReader reader(buffer);

	if (buffer->Empty())
		return IMAGE_FORMAT::UNDEFINED;

	string sample(reader.ReadArray<char>(4), 4);

	if (sample.substr(0, 2) == "BM")
		return IMAGE_FORMAT::BMP;;

	if (sample.substr(0, 3) == "PNG")
		return IMAGE_FORMAT::PNG;

	return IMAGE_FORMAT::UNDEFINED;
}

smart_ptr<Texture> TextureLoader::LoadBMP(Buffer* buffer) {
	BufferReader reader(buffer);
	reader.head(2);

	// Header
	unsigned short bmp_file_size = reader.Read<unsigned>();
	reader.Skip(4);
	unsigned short bmp_offset = reader.Read<unsigned>();

	// load header core 
	BMPHeader header;
	header.header_size = reader.Read<unsigned>();

	decodeBitmapHeader(header, reader);

	// Color table

	if (!(header.header_size == 12 || header.header_size == 40 || header.header_size == 108 ||
		header.header_size == 124 || header.header_size == 52 || header.header_size == 56)) {
		global_log.Report<LogCode::ERROR>("Unsupported BMP header", __FUNCTION__, __LINE__);
		return nullptr;
	}

	std::vector<GLuint> colorTable;

	for (unsigned x = 0; x < header.color_palette_count; ++x) {
		ColorDefinition def;
		switch (header.bit_depth) {
		case 24:
			def.r = reader.Read<char>();
			def.g = reader.Read<char>();
			def.b = reader.Read<char>();
			break;
		case 32:
			def.r = reader.Read<char>();
			def.g = reader.Read<char>();
			def.b = reader.Read<char>();
			def.a = reader.Read<char>();
			break;
		default:
			global_log.Report<LogCode::ERROR>("BMP format not supported: supported bit_depths: 24, 32", __FUNCTION__, __LINE__);
			return nullptr;
		}
		colorTable.push_back(
			toColor(
			header.r_bitmask,
			header.g_bitmask,
			header.b_bitmask,
			header.a_bitmask,
			def)
			);
	}

	Buffer texture_data(header.bmp_height * header.bmp_width * sizeof(GLuint));
	BufferWriter writer(&texture_data);
	reader.head(bmp_offset);

	if (header.color_palette_count == 0) {
		ColorDefinition def;
		switch (header.bit_depth) {
		case 24:
			for (unsigned long h_pos = header.bmp_height; h_pos > 0; --h_pos) {
				for (unsigned long w_pos = 0; w_pos < header.bmp_width; ++w_pos) {
					def.b = reader.Read<char>();
					def.g = reader.Read<char>();
					def.r = reader.Read<char>();
					def.a = 0;
					writer.Write<GLuint>(toColor(header.r_bitmask, header.g_bitmask, header.b_bitmask, header.a_bitmask, def));
				}
				// padding to a multiple of 4 bytes 
				reader.head(header.bmp_width % 4);
			}
			break;
		case 32:
			for (unsigned h_pos = 0; h_pos < header.bmp_height; ++h_pos)
				for (unsigned w_pos = 0; w_pos < header.bmp_width; ++w_pos) {
					def.r = reader.Read<char>();
					def.g = reader.Read<char>();
					def.b = reader.Read<char>();
					def.a = reader.Read<char>();
					writer.Write<GLuint>(toColor(header.r_bitmask, header.g_bitmask, header.b_bitmask, header.a_bitmask, def));
					break;
				}
			break;
		}
	}
	else
		switch (header.bit_depth) {
		case 24:
			writer.Write<GLuint>(colorTable.at(reader.ReadBits(24)));
			reader.Skip(3);
			break;
		case 32:
			writer.Write<GLuint>(colorTable.at(reader.Read<unsigned>()));
			writer.Write<Texture>(Texture(1, { 1, 1 }));
			break;
		}

	GLuint texture_id;
	glGenTextures(1, &texture_id);

	glBindTexture(GL_TEXTURE_2D, texture_id);

	if (header.bit_depth == 24)
		glTexImage2D(GL_TEXTURE_2D,
		0, GL_RGB,
		header.bmp_width,
		header.bmp_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE, texture_data.data());
	else if (header.bit_depth == 32)
		glTexImage2D(GL_TEXTURE_2D,
		0, GL_RGBA,
		header.bmp_width,
		header.bmp_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE, texture_data.data());

	// reset state
	glBindTexture(GL_TEXTURE_2D, 0);


	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		global_log.Report<LogCode::ERROR>(string((char*)glewGetErrorString(err)) + ": Unable to load texture", __FUNCTION__, __LINE__);
		glDeleteTextures(1, &texture_id);
	}

	return new Texture(texture_id, Vector2<unsigned>((unsigned)header.bmp_width, (unsigned)header.bmp_height));
}

smart_ptr<Texture> TextureLoader::LoadTexture(Buffer* buffer) {
	smart_ptr<Texture> tex;

	if (buffer->Empty()) {
		global_log.Report<LogCode::ERROR>("Empty buffer passed to TextureLoader, did file load correctly?\0", __FUNCTION__, __LINE__);
		return nullptr;
	}

	switch (QueryFormat(buffer)) {
	case IMAGE_FORMAT::BMP:
		tex = LoadBMP(buffer);
		break;
	default:
		global_log.Report<LogCode::ERROR>("Unknown image format", __FUNCTION__, __LINE__);
		return nullptr;
		break;
	}
	tex->SetBorder(GL_REPEAT);
	tex->SetFilter(GL_LINEAR);
	return tex;
}

smart_ptr<Texture> TextureLoader::LoadTexture(Buffer&& buffer) {
	return LoadTexture(&buffer);
}
