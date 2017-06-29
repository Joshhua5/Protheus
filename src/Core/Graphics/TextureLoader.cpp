#include "TextureLoader.h"
#include "extern/picopng.cpp"

using namespace Pro;
using namespace Util;
using namespace Graphics;
using namespace Math;

#ifdef PRO_DEBUG
	static bool bmp_debugging = false;
#endif

	struct ColorDefinition {
		union {
			struct { GLubyte r, g, b, a; };
			unsigned bytes;
		};
	};

inline GLuint toColor(const BMPHeader& header,const ColorDefinition& def) { 
	GLuint out = 0;
	GLuint offset;
	switch (header.compression_method) {
	case 0: // BI_RGB 
		out |= def.r;
		offset = header.r_bitmask;
		out |= def.g << offset;
		offset += header.g_bitmask;
		out |= def.b << offset;
		offset += header.b_bitmask;
		out |= def.a << offset;
		return out;
	case 3: // BI_BITFEILDS
		out |= def.bytes & header.a_bitmask;
		out |= def.bytes & header.r_bitmask;
		out |= def.bytes & header.g_bitmask;
		out |= def.bytes & header.b_bitmask; 
		return out;
	} 
	return out;
}

//! Offset in bytes from the start of the texel
inline ColorDefinition toColorDef(GLuint color) {
	ColorDefinition def;
	def.r = color;
	def.g = color >> 8;
	def.b = color >> 16;
	def.a = color >> 24;
	return def;
}

IMAGE_FORMAT TextureLoader::QueryFormat(const Buffer* buffer) {
	// check for BMP
	BufferReader reader(buffer);

	if (buffer->Empty())
		return IMAGE_FORMAT::UNDEFINED;

	string sample(reader.ReadArray<char>(4), 4);

	if (sample.substr(0, 2) == "BM")
		return IMAGE_FORMAT::BMP;;

	if (sample.substr(0, 4) == "‰PNG")
		return IMAGE_FORMAT::PNG;

	return IMAGE_FORMAT::UNDEFINED;
}

void LoadBMPHeader(BufferReader& reader, BMPHeader& header) {
	reader.head(2);
	header.file_size = reader.Read<unsigned>(false);
	// Reserved 4 bytes
	reader.Skip(4);
	header.bmp_data_position = reader.Read<unsigned>(false);
	// load header core
	header.header_size = reader.Read<unsigned>(false);
	{
		switch (header.header_size) {
		case 52: // BITMAPv2INFOHEADER
		case 56: // BITMAPv3INFOHEADER
			header.width = reader.Read<unsigned>(false);
			header.height = reader.Read<unsigned>(false);
			header.color_planes = reader.Read<unsigned short>(false);
			header.bit_depth = reader.Read<unsigned short>(false);
			header.compression_method = reader.Read<unsigned>(false);
			header.bmp_image_size = reader.Read<unsigned>(false);
			header.pixel_per_meter_hor = reader.Read<unsigned>(false);
			header.pixel_per_meter_ver = reader.Read<unsigned>(false);
			header.color_palette_count = reader.Read<unsigned>(false);
			header.important_colors = reader.Read<unsigned>(false);
			header.r_bitmask = reader.Read<unsigned>(false);
			header.g_bitmask = reader.Read<unsigned>(false);
			header.b_bitmask = reader.Read<unsigned>(false);
			if (header.header_size == 56)
				header.a_bitmask = reader.Read<unsigned>(false);
			break;
		case 64: // OS22XBITMAPHEADER
			break;
		default:
			if (header.header_size == 12) { // BITMAPCOREHEADER
				header.width = reader.Read<unsigned short>(false);
				header.height = reader.Read<unsigned short>(false);
				header.color_planes = reader.Read<unsigned short>(false);
				header.bit_depth = reader.Read<unsigned short>(false);
			}
			// BITMAPINFOHEADER
			if (header.header_size == 40 || header.header_size == 108 || header.header_size == 124) {
				header.width = reader.Read<int>(false);
				header.height = reader.Read<int>(false);
				header.color_planes = reader.Read<unsigned short>(false);
				header.bit_depth = reader.Read<unsigned short>(false);
				header.compression_method = reader.Read<unsigned>(false);
				header.bmp_image_size = reader.Read<unsigned>(false);
				header.pixel_per_meter_hor = reader.Read<int>(false);
				header.pixel_per_meter_ver = reader.Read<int>(false);
				header.color_palette_count = reader.Read<unsigned>(false);
				header.important_colors = reader.Read<unsigned>(false);
			}
			// BITMAPV4HEADER
			if (header.header_size == 108 || header.header_size == 124) {
				header.r_bitmask = reader.Read<unsigned>(false);
				header.g_bitmask = reader.Read<unsigned>(false);
				header.b_bitmask = reader.Read<unsigned>(false);
				header.a_bitmask = reader.Read<unsigned>(false);
				header.color_space_type = reader.Read<unsigned>(false);
			}
			// BITMAPV5HEADER
			if (header.header_size == 124) {
				header.bV5Intent = reader.Read<unsigned>(false);
				header.bV5ProfileData = reader.Read<unsigned>(false);
				header.bV5ProfileSize = reader.Read<unsigned>(false);
			}
		}
	}

	switch (header.header_size) {
	case 12:
		header.pixel_size = 3;
	default:
		header.pixel_size = 4;
	}

	header.CorrectEndianess();

	if (bmp_debugging)
		global_log.Report<LogCode::MESSAGE>(header.DebugString(), __FUNCTION__, __LINE__);
}

std::vector<GLuint> LoadBMPColorTable(BufferReader& reader, BMPHeader& header) {
	// Unused def members are left undefined, and therefor must be specifically ignored
	// in the next stage while using the color table 
	//case 8:
	//	for (unsigned x = 0; x < header.color_palette_count; ++x) {
	//		def.r = reader.Read<char>(false);
	//		colorTable.push_back(toColor(header, def));
	//	} 
	std::vector<GLuint> colorTable;

	if (header.color_palette_count != 0) {
		colorTable.reserve(header.color_palette_count);

		ColorDefinition def;
		def.bytes = 0;
		switch (header.header_size) {
		case 12:
			for (unsigned x = 0; x < header.color_palette_count; ++x) {
				def.r = reader.Read<char>(false);
				def.g = reader.Read<char>(false);
				def.b = reader.Read<char>(false);
				colorTable.push_back(toColor(header, def));
			}
			break;
		default: 
			for (unsigned x = 0; x < header.color_palette_count; ++x) {
				// We can consolidate 4 one byte reads into a single 4byte read
				def.bytes = reader.Read<int>(false);
				colorTable.push_back(toColor(header, def));
			}
		}
	}
	return std::move(colorTable);
}

void LoadWithColorTable(BufferWriter& writer, BufferReader& reader, std::vector<GLuint>& colorTable, BMPHeader& header) {
	ColorDefinition def;
	for (unsigned long h_pos = 0; h_pos < header.height; ++h_pos) {
		for (unsigned long w_pos = 0; w_pos < header.width; ++w_pos) {
			switch (header.bit_depth) {
			case 8:
				def = toColorDef(colorTable[reader.Read<unsigned char>(false)]);
				writer.Write<GLchar>(def.r);
				writer.Write<GLchar>(def.g);
				writer.Write<GLchar>(def.b);
				break;
			case 24:
				def = toColorDef(colorTable.at((unsigned)reader.ReadBits(24)));
				reader.Skip(3);
				writer.Write<GLchar>(def.r);
				writer.Write<GLchar>(def.g);
				writer.Write<GLchar>(def.b);
				break;
			case 32:
				writer.Write<GLuint>(colorTable.at(reader.Read<unsigned>(false)));
				break;
			}
		}
	}
}

void LoadFromData(BufferWriter& writer, BufferReader& reader, BMPHeader& header) {
	ColorDefinition def;
	def.bytes = 0;
	switch (header.bit_depth) {
	case 8:
		for (unsigned long h_pos = 0; h_pos < header.height; ++h_pos) {
			for (unsigned long w_pos = 0; w_pos < header.width; ++w_pos) {
				char color = reader.Read<char>(false);
				def.r = def.g = def.b = color;
				writer.Write<GLuint>(toColor(header, def));
				writer.Skip(-1);
			}
			reader.Skip(header.width % 4);
		}
		return;
	case 24:
		for (unsigned long h_pos = 0; h_pos < header.height; ++h_pos) {
			for (unsigned long w_pos = 0; w_pos < header.width; ++w_pos) {
				def.b = reader.Read<char>(false);
				def.g = reader.Read<char>(false);
				def.r = reader.Read<char>(false);
				writer.Write<GLuint>(toColor(header, def));
				// Back the writer by 1, because we only want to write 3 bytes, not 4.
				writer.Skip(-1);
			}
			// padding to a multiple of 4 bytes
			reader.Skip(header.width % 4);
		}
		return;
	case 32:
		for (unsigned long h_pos = 0; h_pos < header.height; ++h_pos) {
			for (unsigned long w_pos = 0; w_pos < header.width; ++w_pos) {
				def.bytes = reader.Read<int>(false);
				writer.Write<GLuint>(toColor(header, def));
			}
			// padding to a multiple of 4 bytes
			reader.Skip(header.width % 4);
		}
		return;
	} 
}
 
Buffer TextureLoader::LoadBMPData(const Buffer *data, BMPHeader& header) {
	if (data->Empty()) {
		global_log.Report<LogCode::FAULT>("Unable to load TextureData, null buffer", __FUNCTION__, __LINE__);
		return Buffer();
	}

	BufferReader reader(data);
	LoadBMPHeader(reader, header);

	// Color table
	if (!(header.header_size == 12 || header.header_size == 40 || header.header_size == 108 ||
		header.header_size == 124 || header.header_size == 52 || header.header_size == 56)) {
		global_log.Report<LogCode::FAULT>("Unsupported BMP header", __FUNCTION__, __LINE__);
		return Buffer();
	}

	std::vector<GLuint> colorTable = LoadBMPColorTable(reader, header);
	 
	Buffer texture_data(header.height * header.width * sizeof(GLuint));
	BufferWriter writer(&texture_data);
	reader.head(header.bmp_data_position);
	 
	ColorDefinition def; 
	def.bytes = 0;
	if (header.color_palette_count == 0)
		LoadFromData(writer, reader, header);
	else 
		LoadWithColorTable(writer, reader, colorTable, header);

	// Compress the texture size
	texture_data.Resize(writer.head());
	return texture_data;
}

Buffer TextureLoader::LoadPNGData(const Buffer* buffer, PNGHeader& header) {
	std::vector<unsigned char> out_image; 
	int error = decodePNG(out_image, header.width, header.height, buffer->data<const unsigned char>(), buffer->size());
    if(error != 0)
        global_log.Report<LogCode::FAULT>("Unknown error inside of PNG Loading", __FUNCTION__, __LINE__);
	// There's a full copy of the texture here, because of a interface issue between std::vector and Buffer
	return Buffer(out_image.data(), out_image.size(), true);
}

shared_ptr<Texture> TextureLoader::LoadBMP(const Buffer* buffer) {
	BMPHeader header;
	Buffer data = LoadBMPData((Buffer*)buffer, header);
	return LoadTextureFromRaw(data, header);
}

shared_ptr<Texture> TextureLoader::LoadPNG(const Buffer* buffer) {
	PNGHeader header; 
	Buffer data = LoadPNGData(buffer, header);
	return LoadTextureFromRaw(data, header);
}

shared_ptr<Texture> InitializeTexture(const Buffer& data, GLenum internal, GLenum external, GLenum depth, int width, int height) {
	GLuint texture_id;
	glGenTextures(1, &texture_id);

	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(GL_TEXTURE_2D,
		0, internal,
		(int)width,
		(int)height,
		0, external,
		depth, data.data());

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		global_log.Report<LogCode::FAULT>("OpenGL Error Code: " + to_string(err) + ": Unable to load texture", __FUNCTION__, __LINE__);
		glDeleteTextures(1, &texture_id);
	}

    shared_ptr<Texture> texture(new Texture(texture_id, Vector2<unsigned>(width, height)));
	texture->SetBorder(GL_REPEAT);
	texture->SetFilter(GL_LINEAR);
	return texture;
}

shared_ptr<Texture> TextureLoader::LoadTextureFromRaw(const Buffer& data, const BMPHeader& header) {
	GLenum internal_texture_type;
	GLenum external_texture_type;
	GLenum color_depth;

	switch (header.bit_depth) { 
	case 8: 
	case 24:
		internal_texture_type = GL_RGB;
		external_texture_type = GL_RGB;
		break;
	case 32:
		internal_texture_type = GL_RGBA;
		external_texture_type = GL_RGBA;
		break;
	default:
		global_log.Report<LogCode::FAULT>("Unable to load texture, unknown header.bit_depth of " +
			std::to_string(header.bit_depth), __FUNCTION__, __LINE__);
		return nullptr;
	}

	switch (header.r_bitmask) {
	case 8:
		color_depth = GL_UNSIGNED_BYTE;
		break;
	case 16:
		color_depth = GL_UNSIGNED_SHORT;
		break;
	case 32:
		color_depth = GL_UNSIGNED_INT;
		break;
	default:
		global_log.Report<LogCode::FAULT>("Unable to load texture, unknown header.r_bitmask of " +
			std::to_string(header.r_bitmask), __FUNCTION__, __LINE__);
		return nullptr;
	}
	return InitializeTexture(data, internal_texture_type, external_texture_type, color_depth, (int)header.width, (int)header.height);
}

shared_ptr<Texture> TextureLoader::LoadTextureFromRaw(const Buffer& data, const PNGHeader& header) {
	// A PNG is always loaded as RGBA32
	if (data.size() == 0 ||
		header.width == 0 ||
		header.height == 0)
		return nullptr; 
	return InitializeTexture(data, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, (int)header.width, (int)header.height);
}



shared_ptr<Texture> TextureLoader::LoadTexture(const Util::Buffer& buffer) {

	if (buffer.Empty()) {
		global_log.Report<LogCode::FAULT>("Empty buffer passed to TextureLoader, did file load correctly?\0", __FUNCTION__, __LINE__);
		return nullptr;
	}

	shared_ptr<Texture> tex;

	switch (QueryFormat((Buffer*)&buffer)) {
	case IMAGE_FORMAT::BMP:
		tex = LoadBMP((Buffer*)&buffer);
		break;
	case IMAGE_FORMAT::PNG:
		tex =  LoadPNG((Buffer*)&buffer);
		break;
	default:
		global_log.Report<LogCode::FAULT>("Unknown image format", __FUNCTION__, __LINE__);
		return nullptr;
		break;
	}
	tex->SetBorder(GL_REPEAT);
	tex->SetFilter(GL_LINEAR);
	return tex;
} 

void TextureLoader::EnableDebugging() {
	bmp_debugging = true; 
}

void TextureLoader::DisableDebugging() {
	bmp_debugging = false; 
}