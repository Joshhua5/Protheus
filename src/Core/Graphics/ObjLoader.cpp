#include "MeshLoader.h"

using namespace Pro;
using namespace Graphics;
using namespace Util; 

static inline unsigned process_linei(const string& format, const BufferReader& in, BufferWriter& out) {
	int face[12];

	auto ret = sscanf(in.ReadRaw(), format.data(),
		face + 0, face + 1, face + 2,
		face + 3, face + 4, face + 5,
		face + 6, face + 7, face + 8,
		face + 9, face + 10, face + 11);
	for (unsigned char x = 0; x < ret; ++x)
		*(face + x) -= 1;

	out.WriteElements<int>(face, ret);
	return ret;
}

static bool processOBJ(std::vector<MeshObjectTemp>& objects, Buffer* file, BufferWriter& vertex_writer, BufferWriter& normal_writer, BufferWriter& tex_coord_writer) {
	BufferReader reader(file);

	float vertex[4];
	bool first_object = true;
	memset(vertex, 0, sizeof(float) * 4);

	MeshObjectTemp* object = &objects.back();
	object->temp = new Buffer(file->size() / 10);
	object->tempWriter = new BufferWriter(object->temp);

	while (reader.HasNext()) {
		auto line = reader.ReadDelim('\n', false);
		BufferReader liner(&line);

		// Check that the next line is creating an object if one hasn't been
		// skip the line if one isn't created  


		switch (line.data<char>()[0]) {
		case 'o': {
			liner.head(2);
			// pointer will break outside of this function

			auto nameBuf = liner.ReadDelim('\n', false);

			if (first_object) {
				object->name = string(nameBuf.data<char>(), nameBuf.size() - 1);
				first_object = false;
			}
			else
				objects.push_back(std::move(MeshObjectTemp(string(nameBuf.data<char>(), nameBuf.size() - 1), 0, 0)));

			object = &objects.back();
			object->temp = new Buffer(file->size() / 10);
			object->tempWriter = new BufferWriter(object->temp);
		}
				  break;
		case 'f':

			if (line.Count<char>(' ') != 3) {
				Pro::global_log.Report<LogCode::FAULT>("Object file contains a face which isn't of valid type: Triangles", __FUNCTION__, __LINE__);
				return false;
			}
			object->vertex_count += 3;

			if (liner.Contains<char>("//", 2) != -1) {
				// Vector//Normal  
				process_linei("f %i//%i %i//%i %i//%i", liner, *object->tempWriter);
				object->face_format = FACE_FORMAT::VERTEX_NORMAL;
				object->has_normals = true;
				break;
			}
			switch (line.Count<char>('/')) {
			case 0:
				// simple vertex 
				process_linei("f %i %i %i", liner, *object->tempWriter);
				object->face_format = FACE_FORMAT::VERTEX;
				break;
			case 3:
				// vertex/uv   
				process_linei("f %i/%i %i/%i %i/%i", liner, *object->tempWriter);
				object->face_format = FACE_FORMAT::VERTEX_UV;
				object->has_tex_coord = true;
				break;
			case 6:
				// vertex/uv/normal
				process_linei("f %i/%i/%i %i/%i/%i %i/%i/%i", liner, *object->tempWriter);
				object->face_format = FACE_FORMAT::VERTEX_UV_NORMAL;
				object->has_tex_coord = object->has_normals = true;
				break;
			}

			break;

		case 'v':
			switch (line.data<char>()[1]) {
			case 't':
				object->tex_coord_per_vertex =
					sscanf(liner.ReadRaw(), "vt %f %f %f %f",
						vertex + 0, vertex + 1, vertex + 2, vertex + 3);

				tex_coord_writer.WriteElements<float>(vertex, object->tex_coord_per_vertex);
				break;
			case 'n':
				sscanf(liner.ReadRaw(), "vn %f %f %f",
					vertex + 0, vertex + 1, vertex + 2);
				normal_writer.WriteElements<float>(vertex, 3);

				break;
			case ' ':
				object->floats_per_vertex = sscanf(liner.ReadRaw(), "v %f %f %f %f",
					vertex + 0, vertex + 1, vertex + 2, vertex + 3);

				vertex_writer.WriteElements<float>(vertex, object->floats_per_vertex);
				break;
			}
		}
	}
	// Set the last size of the object
	object->size = object->vertex_count - object->start;
	return true;
}

std::shared_ptr<Mesh> MeshLoader::LoadOBJ(Buffer* buffer) {
	BufferReader reader(buffer);
	Buffer verticies(buffer->size() / 3);
	BufferWriter vertex_writer(&verticies);
	Buffer normals(buffer->size() / 3);
	BufferWriter normal_writer(&normals);
	Buffer tex_coords(buffer->size() / 3);
	BufferWriter tex_coord_writer(&tex_coords);

	// code below uses a pointer, 
	// kept pointer so I didn't have to edit
	std::vector<MeshObjectTemp> object;

	object.push_back(MeshObjectTemp("", 0, 0));

	if (processOBJ(object, buffer, vertex_writer, normal_writer, tex_coord_writer) == false) {
		global_log.Report<LogCode::FAULT>("Unable to load object file\0", __FUNCTION__, __LINE__);
		return nullptr;
	}

	// populate the size of the last object 

	// Pack verticies 
	Buffer packed(verticies.size() + normals.size() + tex_coords.size());
	Buffer elements(verticies.size());
	BufferWriter element_writer(&elements);
	BufferWriter packed_writer(&packed);

	Buffer search_face(4 * sizeof(float));
	BufferWriter search_face_writer(&search_face);
	// Okay because indicies will read ahead of the writer
	// Perfect for OpenCL

	unsigned current_element = 0;

	struct Vertex {
		int vertex = -1;
		int normal = -1;
		int tex_coord = -1;

		inline bool operator==(const Vertex& face) {
			return vertex == face.vertex && normal == face.normal && tex_coord == face.tex_coord;
		}
	};

	std::vector<Vertex> current_verticies;

	for (auto& obj : object) {
		BufferReader reader(obj.temp);

		// get the next faces
		// compair to see if they have already be defined
		// if true then use it's index in the element buffer
		// otherwise add the verticies into the buffer 
		// and insert the new index into the element

		for (unsigned face_id = 0; face_id < obj.vertex_count; ++face_id) {
			// Get Faces
			Vertex vertex;
			bool existing = false;

			switch (obj.face_format) {
			case FACE_FORMAT::VERTEX:
				vertex.vertex = reader.Read<int>();
				break;
			case FACE_FORMAT::VERTEX_NORMAL:
				vertex.vertex = reader.Read<int>();
				vertex.normal = reader.Read<int>();
				break;
			case FACE_FORMAT::VERTEX_UV:
				vertex.vertex = reader.Read<int>();
				vertex.tex_coord = reader.Read<int>();
				break;
			case FACE_FORMAT::VERTEX_UV_NORMAL:
				vertex.vertex = reader.Read<int>();
				vertex.tex_coord = reader.Read<int>();
				vertex.normal = reader.Read<int>();
				break;
			case FACE_FORMAT::UNDEFINED:
				global_log.Report<LogCode::FAULT>("Unable to load Model, undefined face format", __FUNCTION__, __LINE__);
				// Buffers will clean them selfes 
				return nullptr;
			}

			// Check if face has already been defined
			//for (unsigned x = 0; x < current_verticies.size(); ++x)
			//if (current_verticies[x] == vertex) {
			//	existing = true;
			//	element_writer.write<unsigned>(x);
			//	break;
			//}

			// If no duplicates, add into the verticies and element buffer
			if (existing == false) {
				packed_writer.WriteElements<float>(verticies.data<float>() + vertex.vertex * obj.floats_per_vertex, obj.floats_per_vertex);
				switch (obj.face_format) {
				case FACE_FORMAT::VERTEX_NORMAL:
					packed_writer.WriteElements<float>(normals.data<float>() + vertex.normal * 3, 3);
					break;
				case FACE_FORMAT::VERTEX_UV:
					packed_writer.WriteElements<float>(tex_coords.data<float>() + vertex.tex_coord * obj.tex_coord_per_vertex, obj.tex_coord_per_vertex);
					break;
				case FACE_FORMAT::VERTEX_UV_NORMAL:
					packed_writer.WriteElements<float>(tex_coords.data<float>() + vertex.tex_coord  * obj.tex_coord_per_vertex, obj.tex_coord_per_vertex);
					packed_writer.WriteElements<float>(normals.data<float>() + vertex.normal * 3, 3);
					break;
				default:
					break;
				}
				current_verticies.push_back(vertex);
				element_writer.Write<unsigned>(current_element++);
			}
		}
		obj.vertex_count = (unsigned)current_verticies.size();
	}

	// Special case for the first object's start
	// Also clean up the temp files
	delete object[0].temp;
	delete object[0].tempWriter;
	object[0].size = object[0].vertex_count;

	// Update the start and size of each object
	for (unsigned x = 1; x < object.size(); x++) {
		object[x].start = object[x - 1].vertex_count;
		object[x].size = object[x].vertex_count - object[x - 1].vertex_count;

		delete object[x].temp;
		delete object[x].tempWriter;
	}

	// Create VBO in OpenGL

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, packed_writer.head(), packed.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (glGetError() != GL_NO_ERROR) {
		global_log.Report<LogCode::FAULT>("Unable to load OBJ Model: Create Array Buffer", __FUNCTION__, __LINE__);
		return nullptr;
	}

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_writer.head(), elements.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (glGetError() != GL_NO_ERROR) {
		global_log.Report<LogCode::FAULT>("Unable to load OBJ Model: Create Element Array", __FUNCTION__, __LINE__);
		return nullptr;
	}

	// TODO Check the UML of Mesh and MeshObject to see what's duplicated
	shared_ptr<Mesh> m = std::make_shared<Mesh>(vbo, ebo);
	for (unsigned x = 0; x < object.size(); ++x)
		m->AttachObject(std::move(object[x]).Export());

	return std::move(m);
}