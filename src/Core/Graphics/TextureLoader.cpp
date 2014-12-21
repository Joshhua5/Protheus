#include "TextureLoader.h"

using namespace Pro;

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

inline void decodeBitmapHeader(BMPHeader& header, BufferReader& reader) {
	switch (header.header_size) {
	case 52: // BITMAPv2INFOHEADER 
	case 56: // BITMAPv3INFOHEADER
		header.bmp_width = reader.read<unsigned long>();
		header.bmp_height = reader.read<unsigned long>();
		header.color_planes = reader.read<unsigned short>();
		header.bit_depth = reader.read<unsigned short>();
		header.compression_method = reader.read<unsigned>();
		header.bmp_image_size = reader.read<unsigned>();
		header.pixel_per_meter_hor = reader.read<long>();
		header.pixel_per_meter_ver = reader.read<long>();
		header.color_palette_count = reader.read<unsigned>();
		header.important_colors = reader.read<unsigned>();
		header.r_bitmask = reader.read<unsigned>();
		header.g_bitmask = reader.read<unsigned>();
		header.b_bitmask = reader.read<unsigned>();
		if (header.header_size == 56)
			header.a_bitmask = reader.read<unsigned>();
		break;
	case 64: // OS22XBITMAPHEADER 
		break;
	default:
		if (header.header_size == 12) {
			header.bmp_width = reader.read<unsigned short>();
			header.bmp_height = reader.read<unsigned short>();
			header.color_planes = reader.read<unsigned short>();
			header.bit_depth = reader.read<unsigned short>();
		}
		// BITMAPINFOHEADER
		if (header.header_size == 40 || header.header_size == 108 || header.header_size == 124) {
			header.bmp_width = reader.read<int>();
			header.bmp_height = reader.read<int>();
			header.color_planes = reader.read<unsigned short>();
			header.bit_depth = reader.read<unsigned short>();
			header.compression_method = reader.read<unsigned>();
			header.bmp_image_size = reader.read<unsigned>();
			header.pixel_per_meter_hor = reader.read<int>();
			header.pixel_per_meter_ver = reader.read<int>();
			header.color_palette_count = reader.read<unsigned>();
			header.important_colors = reader.read<unsigned>();
		}
		// BITMAPV4HEADER   
		if (header.header_size == 108 || header.header_size == 124) {
			header.r_bitmask = reader.read<unsigned>();
			header.g_bitmask = reader.read<unsigned>();
			header.b_bitmask = reader.read<unsigned>();
			header.a_bitmask = reader.read<unsigned>();
			header.color_space_type = reader.read<unsigned>();
		}
		// BITMAPV5HEADER  
		if (header.header_size == 124) {
			header.bV5Intent = reader.read<unsigned>();
			header.bV5ProfileData = reader.read<unsigned>();
			header.bV5ProfileSize = reader.read<unsigned>();
		}
	}
}


IMAGE_FORMAT TextureLoader::queryFormat(Buffer* buffer) {
	// check for BMP
	BufferReader reader(buffer);

	if (buffer->isEmpty())
		return IMAGE_FORMAT::UNDEFINED;

	string sample(reader.read_array<char>(4), 4);

	if (sample.substr(0, 2) == "BM")
		return IMAGE_FORMAT::BMP;;

	if (sample.substr(0, 3) == "PNG")
		return IMAGE_FORMAT::PNG;

	return IMAGE_FORMAT::UNDEFINED;
}

smart_pointer<Texture> TextureLoader::loadBMP(Buffer* buffer) {
	BufferReader reader(buffer);
	reader.setPosition(2);

	// Header
	unsigned short bmp_file_size = reader.read<unsigned>();
	reader.skip(4);
	unsigned short bmp_offset = reader.read<unsigned>();

	// load header core 
	BMPHeader header;
	header.header_size = reader.read<unsigned>();
 
	decodeBitmapHeader(header, reader);

	// Color table

	if (!(header.header_size == 12   || header.header_size == 40 || header.header_size == 108 ||
		header.header_size == 124  || header.header_size == 52 || header.header_size == 56)) {
		error.reportError("Unsupported BMP header");
		return nullptr;
	}

	std::vector<GLuint> colorTable;

	for (unsigned x = 0; x < header.color_palette_count; ++x) {
		ColorDefinition def;
		switch (header.bit_depth) {
		case 24:
			def.r = reader.read<char>();
			def.g = reader.read<char>();
			def.b = reader.read<char>();
			break;
		case 32:
			def.r = reader.read<char>();
			def.g = reader.read<char>();
			def.b = reader.read<char>();
			def.a = reader.read<char>();
			break;
		default:
			error.reportError("BMP format not supported: supported bit_depths: 24, 32");
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
	reader.setPosition(bmp_offset);

	if (header.color_palette_count == 0) {
		ColorDefinition def;
		switch (header.bit_depth) {
		case 24:
			for (unsigned long h_pos = header.bmp_height; h_pos > 0; --h_pos) {
				for (unsigned long w_pos = 0; w_pos < header.bmp_width; ++w_pos) {
					def.b = reader.read<char>();
					def.g = reader.read<char>();
					def.r = reader.read<char>();
					def.a = 0;
					writer.write<GLuint>(toColor(header.r_bitmask, header.g_bitmask, header.b_bitmask, header.a_bitmask, def));
				}
				// padding to a multiple of 4 bytes 
				reader.skip(header.bmp_width % 4);
			}
			break;
		case 32:
			for (unsigned h_pos = 0; h_pos < header.bmp_height; ++h_pos)
				for (unsigned w_pos = 0; w_pos < header.bmp_width; ++w_pos) {
					def.r = reader.read<char>();
					def.g = reader.read<char>();
					def.b = reader.read<char>();
					def.a = reader.read<char>();
					writer.write<GLuint>(toColor(header.r_bitmask, header.g_bitmask, header.b_bitmask, header.a_bitmask, def));
					break;
				}
			break;
		}
	}
	else
		switch (header.bit_depth) {
		case 24:
			writer.write<GLuint>(colorTable.at(reader.read_bits(24)));
			reader.skip(3);
			break;
		case 32:
			writer.write<GLuint>(colorTable.at(reader.read<unsigned>()));
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
	return new Texture(texture_id, Vector2<unsigned>((unsigned)header.bmp_width, (unsigned)header.bmp_height));
}

smart_pointer<Texture> TextureLoader::loadTexture(Buffer* buffer) {
	smart_pointer<Texture> tex;

	if (buffer->isEmpty()) {
		error.reportError("Empty buffer passed to TextureLoader, did file load correctly?\0");
		return nullptr;
	}

	switch (queryFormat(buffer)) {
	case IMAGE_FORMAT::BMP:
		tex = loadBMP(buffer);
		break;
	default:
		error.reportError("Unknown image format");
		return nullptr;
		break;
	}
	tex->setBorder(GL_REPEAT);
	tex->setFilter(GL_LINEAR);
	return tex;
}

smart_pointer<Texture> TextureLoader::loadTexture(Buffer&& buffer) {
	return loadTexture(&buffer);
}
