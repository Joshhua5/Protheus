#include "MeshLoader.h"

using namespace Pro;
using namespace Util;

enum struct FACE_FORMAT {
	VERTEX,
	VERTEX_NORMAL,
	VERTEX_UV_NORMAL,
	VERTEX_UV
};

MODEL_FORMAT  MeshLoader::queryFormat(CBuffer* buffer) {

	if (buffer->isEmpty())
		return MODEL_FORMAT::UNDEFINED;

	//switch () {


	//default:
	//	return MODEL_FORMAT::UNDEFINED;
	//}
	return MODEL_FORMAT::UNDEFINED;
} 

inline unsigned process_linei(const string& format, BufferReader& in, BufferWriter& out, unsigned& face_count) {
	int face[12];
	// vertex/uv/normal 
	// TEST does atoi stop at the /
	auto ret = sscanf(in.read_raw(), format.data(),
		face + 0, face + 1, face + 2,
		face + 3, face + 4, face + 5,
		face + 6, face + 7, face + 8,
		face + 9, face + 10, face + 11);
	for (unsigned char x = 0; x < ret; ++x)
		*(face + x) -= 1;

	out.write_elements<int>(face, ret);
	++face_count;
	return ret;
}


Mesh* MeshLoader::loadOBJ(CBuffer* buffer) {
	BufferReader reader(buffer);

	CBuffer verticies;
	BufferWriter vertex_writer(&verticies);
	CBuffer normals;
	BufferWriter normal_writer(&normals);
	CBuffer tex_coords;
	BufferWriter tex_coord_writer(&tex_coords);
	CBuffer faces;
	BufferWriter face_writer(&faces);
	unsigned verticie_count = 0;
	unsigned normal_count = 0;
	unsigned face_count = 0;
	bool vertex_w = false;
	bool tex_coord_w = false;
	unsigned vertex_per_face = 0;
	unsigned floats_per_vertex = 0;
	unsigned tex_coord_per_face = 0;
	FACE_FORMAT face_format;
	GLenum gl_render_mode;

	// code below uses a pointer, 
	// kept pointer so I didn't have to edit
	std::vector<MeshObject> object;
	 
	float vertex[4]; 
	memset(vertex, 0, sizeof(float) * 4); 
	 

	while (reader.hasNext()) {
		auto line = reader.read_delim('\n', false);
		BufferReader liner(&line);

		// Check that the next line is creating an object if one hasn't been
		// skip the line if one isn't created  


		switch (line.data<char>()[0]) {
		case 'o': {
			liner.setPosition(2);
			// pointer will break outside of this function

			if (object.size() >= 1)
				object.back().size =
				((face_count - object.back().start) * vertex_per_face);

			auto nameBuf = liner.read_delim('\n', false);
			object.push_back(MeshObject(string(nameBuf.data<char>(), nameBuf.size() - 1), face_count, 0));

		}
				  break;

		case 'f':
			// TEST
			// Check for what kind of face

			if (line.count<char>(' ') == 3) {
				gl_render_mode = GL_TRIANGLE_STRIP;
				vertex_per_face = 3;
			}
			else {
				gl_render_mode = GL_QUADS;
				vertex_per_face = 4;
			}

			switch (gl_render_mode) {
			case GL_TRIANGLE_STRIP:
				if (liner.contains<char>("//", 2) != -1) {
					// Vector//Normal  
					process_linei("f %i//%i %i//%i %i//%i", liner, face_writer, face_count);
					face_format = FACE_FORMAT::VERTEX_NORMAL;
					break;
				}
				switch (line.count<char>('/')) {
				case 0:
					// simple vertex 
					process_linei("f %i %i %i", liner, face_writer, face_count);
					face_format = FACE_FORMAT::VERTEX;
					break;
				case 3:
					// vertex/uv   
					process_linei("f %i/%i %i/%i %i/%i", liner, face_writer, face_count);
					face_format = FACE_FORMAT::VERTEX_UV;
					break;
				case 6:
					// vertex/uv/normal 
					// TEST does atoi stop at the /
					process_linei("f %i/%i/%i %i/%i/%i %i/%i/%i", liner, face_writer, face_count);
					face_format = FACE_FORMAT::VERTEX_UV_NORMAL;
					break;
				}
				break;

			case GL_QUADS:
				if (liner.contains<char>("//", 2) != -1) {
					// Vector//Normal   
					process_linei("f %i//%i %i//%i %i//%i %i//%i", liner, face_writer, face_count);
					face_format = FACE_FORMAT::VERTEX_NORMAL;
					break;
				}
				switch (line.count<char>('/')) {
				case 0:
					// simple vertex 
					process_linei("f %i %i %i %i", liner, face_writer, face_count);
					face_format = FACE_FORMAT::VERTEX;
					break;
				case 4:
					// vertex/uv  
					// TEST does atoi stop at the /
					process_linei("f %i/%i %i/%i %i/%i %i/%i", liner, face_writer, face_count);
					face_format = FACE_FORMAT::VERTEX_UV;
					break;
				case 8:
					// vertex/uv/normal 
					// TEST does atoi stop at the /
					process_linei("f %i/%i/%i %i/%i/%i %i/%i/%i %i/%i/%i", liner, face_writer, face_count);
					face_format = FACE_FORMAT::VERTEX_UV_NORMAL;
					break;
				}
				break;
			default:
				error.reportErrorNR("Unsupported face type in .obj\0");
				return nullptr;
			}

			break;

		case 'v':
			switch (line.data<char>()[1]) {
			case 't':
				tex_coord_w = 
					(sscanf(liner.read_raw(), "vt %f %f %f %f",
					vertex + 0, vertex + 1, vertex + 2) == 3);

				if (!tex_coord_w)
					vertex[2] = 0;
				tex_coord_per_face = (tex_coord_w ? 3 : 2);

				tex_coord_writer.write_elements<float>(vertex, tex_coord_per_face);
				break;
			case 'n':
				sscanf(liner.read_raw(), "vn %f %f %f",
					vertex + 0, vertex + 1, vertex + 2);
				normal_writer.write_elements<float>(vertex, 3);

				++normal_count;
				break;
			case ' ':
				vertex_w = (sscanf(liner.read_raw(), "v %f %f %f %f",
					vertex + 0, vertex + 1, vertex + 2, vertex + 3) == 4);
				floats_per_vertex = (vertex_w ? 4 : 3);

				vertex_writer.write_elements<float>(vertex, floats_per_vertex);
				++verticie_count;
				break;
			}
		}
	}
	// populate the size of the last object 
	object.back().size =
		((face_count - object.back().start) * vertex_per_face);

	// Pack verticies 
	CBuffer packed(4000);
	BufferReader face_reader(&faces);
	BufferWriter packed_writer(&packed);
	// okay because indicies will read ahead of the writer 

	if (face_format == FACE_FORMAT::VERTEX)
		packed.init(verticies.data(), verticies.size(), false);
	else { 
		face_writer.reset(); 
		face_reader.reset();
		for (unsigned x = 0; x < face_count * vertex_per_face; ++x)
			switch (face_format) {
			case FACE_FORMAT::VERTEX_NORMAL:
				packed_writer.write_elements<float>(verticies.data<float>() + face_reader.read<int>() * floats_per_vertex, floats_per_vertex);
				packed_writer.write_elements<float>(normals.data<float>() + face_reader.read<int>() * 3, 3);
				face_writer.write<unsigned>(x);
				break;
			case FACE_FORMAT::VERTEX_UV:
				packed_writer.write_elements<float>(verticies.data<float>() + face_reader.read<int>() * floats_per_vertex, floats_per_vertex);
				packed_writer.write_elements<float>(tex_coords.data<float>() + face_reader.read<int>() * tex_coord_per_face, tex_coord_per_face);
				face_writer.write<unsigned>(x);
				break;
			case FACE_FORMAT::VERTEX_UV_NORMAL:
				packed_writer.write_elements<float>(verticies.data<float>() + face_reader.read<int>() * floats_per_vertex, floats_per_vertex);
				packed_writer.write_elements<float>(tex_coords.data<float>() + face_reader.read<int>() * tex_coord_per_face, tex_coord_per_face);

				packed_writer.write_elements<float>(normals.data<float>() + face_reader.read<int>() * 3, 3); 
				face_writer.write<unsigned>(x);
				break;
			}
	}
	// Create VBO in OpenGL

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	  
	glBufferData(GL_ARRAY_BUFFER,packed_writer.getPosition() , packed.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (glGetError() != GL_NO_ERROR) {
		error.reportError("Unable to load OBJ Model: Create Array Buffer");
		return nullptr;
	}

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_writer.getPosition(), faces.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (glGetError() != GL_NO_ERROR) {
		error.reportError("Unable to load OBJ Model: Create Element Array");
		return nullptr;
	}
	Mesh* m = new Mesh(vbo, ebo, gl_render_mode, vertex_w, tex_coord_w, !tex_coords.isEmpty(), !normals.isEmpty());
	for (unsigned x = 0; x < object.size(); ++x)
		m->attachObject(std::move(object[x]));

	return m;
}
 
Mesh* MeshLoader::loadModel(CBuffer* buffer) {
	Mesh* model = nullptr;

	if (buffer->isEmpty()) {
		error.reportError("Empty buffer passed to MeshLoader did file load correctly?\0");
		return nullptr;
	}

	switch (queryFormat(buffer)) {
	case MODEL_FORMAT::OBJ:
		model = loadOBJ(buffer);
		break;
	default:
		error.reportError("Unable to load model: unknown format\0");
		return nullptr;
	}

		error.reportError("Unable to load model: Load fail\0");
	if (model == nullptr) {
		return nullptr;
	}

	return model;
}
Mesh* MeshLoader::loadModel(CBuffer&& buffer) {
	return loadModel(&buffer);
}