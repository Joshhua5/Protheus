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

IMAGE_FORMAT TextureLoader::queryFormat(CBuffer* buffer) {
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

smart_pointer<Texture> TextureLoader::loadBMP(CBuffer* buffer) {
	BufferReader reader(buffer);
	reader.setPosition(2);

	// Header
	unsigned short bmp_file_size = reader.read<unsigned>();
	reader.skip(4);
	unsigned short bmp_offset = reader.read<unsigned>();

	// load header core 
	unsigned header_size = reader.read<unsigned>();
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

	// BITMAPCOREHEADER
	if (header_size == 12) {
		bmp_width = reader.read<unsigned short>();
		bmp_height = reader.read<unsigned short>();
		color_planes = reader.read<unsigned short>();
		bit_depth = reader.read<unsigned short>();
	}
	// BITMAPINFOHEADER
	if (header_size == 40 || header_size == 108 || header_size == 124) {
		bmp_width = reader.read<int>();
		bmp_height = reader.read<int>();
		color_planes = reader.read<unsigned short>();
		bit_depth = reader.read<unsigned short>();
		compression_method = reader.read<unsigned>();
		bmp_image_size = reader.read<unsigned>();
		pixel_per_meter_hor = reader.read<int>();
		pixel_per_meter_ver = reader.read<int>();
		color_palette_count = reader.read<unsigned>();
		important_colors = reader.read<unsigned>();
	}
	// BITMAPV4HEADER   
	if (header_size == 108 || header_size == 124) {
		r_bitmask = reader.read<unsigned>();
		g_bitmask = reader.read<unsigned>();
		b_bitmask = reader.read<unsigned>();
		a_bitmask = reader.read<unsigned>();
		color_space_type = reader.read<unsigned>();
	}
	// BITMAPV5HEADER  
	if (header_size == 124) {
		bV5Intent = reader.read<unsigned>();
		bV5ProfileData = reader.read<unsigned>();
		bV5ProfileSize = reader.read<unsigned>();
	}

	// Color table

	std::vector<GLuint> colorTable;

	for (unsigned x = 0; x < color_palette_count; ++x) {
		ColorDefinition def;
		switch (bit_depth) {
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
			r_bitmask,
			g_bitmask,
			b_bitmask,
			a_bitmask,
			def)
			);
	}

	CBuffer texture_data(bmp_height * bmp_width * sizeof(GLuint));
	BufferWriter writer(&texture_data);
	reader.setPosition(bmp_offset);

	if (color_palette_count == 0) {
		ColorDefinition def;
		switch (bit_depth) {
		case 24:
			for (unsigned long h_pos = bmp_height; h_pos > 0; --h_pos) {
				for (unsigned long w_pos = 0; w_pos < bmp_width; ++w_pos) {
					def.b = reader.read<char>();
					def.g = reader.read<char>();
					def.r = reader.read<char>();
					def.a = 0;
					writer.write<GLuint>(toColor(r_bitmask, g_bitmask, b_bitmask, a_bitmask, def));
				}
				// padding to a multiple of 4 bytes 
				reader.skip(bmp_width % 4);
			}
			break;
		case 32:
			for (unsigned h_pos = 0; h_pos < bmp_height; ++h_pos)
				for (unsigned w_pos = 0; w_pos < bmp_width; ++w_pos) {
					def.r = reader.read<char>();
					def.g = reader.read<char>();
					def.b = reader.read<char>();
					def.a = reader.read<char>();
					writer.write<GLuint>(toColor(r_bitmask, g_bitmask, b_bitmask, a_bitmask, def));
					break;
				}
			break;
		}
	}
	else
		switch (bit_depth) {
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

	if (bit_depth == 24)
		glTexImage2D(GL_TEXTURE_2D,
		0, GL_RGB,
		bmp_width,
		bmp_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE, texture_data.data());
	else if (bit_depth == 32)
		glTexImage2D(GL_TEXTURE_2D,
		0, GL_RGBA,
		bmp_width,
		bmp_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE, texture_data.data());

	// reset state
	glBindTexture(GL_TEXTURE_2D, 0); 
	return new Texture(texture_id, Vector2<unsigned>((unsigned) bmp_width, (unsigned) bmp_height));
}

smart_pointer<Texture> TextureLoader::loadTexture(CBuffer* buffer) {
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

smart_pointer<Texture> TextureLoader::loadTexture(CBuffer&& buffer) {
	return loadTexture(&buffer);
}
 