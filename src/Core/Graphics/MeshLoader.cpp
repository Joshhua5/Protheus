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

Mesh* MeshLoader::loadOBJ(CBuffer* buffer) {
	BufferReader reader(buffer);

	CBuffer verticies;
	BufferWriter vertWriter(&verticies);
	CBuffer normals;
	BufferWriter normWriter(&normals);
	CBuffer uv;
	BufferWriter uvWriter(&uv);
	CBuffer faces;
	BufferWriter faceWriter(&faces);
	unsigned vertexCount = 0;
	unsigned normalCount = 0;
	unsigned faceCount = 0;
	bool containsW = false;
	FACE_FORMAT face_format;
	GLenum face_type;

	// code below uses a pointer, 
	// kept pointer so I didn't have to edit
	std::vector<MeshObject> object;

	int face[12];
	float vertex[16];

	memset(vertex, 0, sizeof(float) * 16);
	memset(face, 0, sizeof(float) * 12);

	MeshObject obj;
	MeshObject* temp = nullptr;

	while (reader.hasNext()) {
		auto line = reader.read_delim('\n', false);
		BufferReader liner(&line);

		// Check that the next line is creating an object if one hasn't been
		// skip the line if one isn't created  


		switch (line.data<char>()[0]) {
		case 'o': {
			liner.reset();
			liner.skip(2);
			// pointer will break outside of this function
			object.push_back(MeshObject());
			auto nameBuf = liner.read_delim('\n', false);
			temp = &object.back();

			temp->name = string(nameBuf.data<char>(), nameBuf.size() - 1);
			temp->start = faceCount; 

			if (object.size() >= 2) {
				temp = &object.at(object.size() - 2);
				temp->size = faceCount - temp->start;
			}
		}
			break;

		case 'f':
			// TEST
			// Check for what kind of face

			if (line.count<char>(' ') == 3)
				face_type = GL_TRIANGLE_STRIP;
			else
				face_type = GL_QUADS;

			switch (face_type)
			{
			case GL_TRIANGLE_STRIP:
				if (liner.contains<char>("//", 2) != -1) {
					// Vector//Normal  
					sscanf(liner.read_raw(),
						"f %i//%i %i//%i %i//%i",
						face + 0, face + 1,
						face + 2, face + 3,
						face + 4, face + 5);

					faceWriter.write_elements<int>(face, 6);

					++faceCount;
					face_format = FACE_FORMAT::VERTEX_NORMAL;
					break;
				}
				switch (line.count<char>('/')) {
				case 0:
					// simple vertex 
					sscanf(liner.read_raw(),
						"%*s %i %i %i",
						face + 0, face + 1,
						face + 2);

					faceWriter.write_elements<int>(face, 3);

					++faceCount;
					face_format = FACE_FORMAT::VERTEX;
					break;
				case 4:
					// vertex/uv  
					// TEST does atoi stop at the /

					sscanf(liner.read_raw(),
						"f %i/%i %i/%i %i/%i",
						face + 0, face + 1,
						face + 2, face + 3,
						face + 4, face + 5);

					faceWriter.write_elements<int>(face, 6);


					++faceCount;
					face_format = FACE_FORMAT::VERTEX_UV;
					break;
				case 8:
					// vertex/uv/normal 
					// TEST does atoi stop at the /

					sscanf(liner.read_raw(),
						"f %i/%i/%i %i/%i/%i %i/%i/%i",
						face + 0, face + 1, face + 2,
						face + 3, face + 4, face + 5,
						face + 6, face + 7, face + 8);

					faceWriter.write_elements<int>(face, 9);

					++faceCount;
					face_format = FACE_FORMAT::VERTEX_UV_NORMAL;
					break;
				}
				break;

			case GL_QUADS:
				if (liner.contains<char>("//", 2) != -1) {
					// Vector//Normal   
					sscanf(liner.read_raw(),
						"f %i//%i %i//%i %i//%i %i//%i",
						face + 0, face + 1,
						face + 2, face + 3,
						face + 4, face + 5,
						face + 6, face + 7);
					faceWriter.write_elements<int>(face, 8);

					++faceCount;
					face_format = FACE_FORMAT::VERTEX_NORMAL;
					break;
				}
				switch (line.count<char>('/')) {
				case 0:
					// simple vertex 
					sscanf(liner.read_raw(),
						"f %i %i %i %i",
						face + 0, face + 1,
						face + 2, face + 3);

					faceWriter.write_elements<int>(face, 4);

					++faceCount;
					face_format = FACE_FORMAT::VERTEX;
					break;
				case 4:
					// vertex/uv  
					// TEST does atoi stop at the /
					sscanf(liner.read_raw(),
						"f %i/%i %i/%i %i/%i %i/%i",
						face + 0, face + 1, face + 2,
						face + 3, face + 4, face + 5,
						face + 6, face + 7);

					faceWriter.write_elements<int>(face, 8);

					++faceCount;
					face_format = FACE_FORMAT::VERTEX_UV;
					break;
				case 8:
					// vertex/uv/normal 
					// TEST does atoi stop at the /
					sscanf(liner.read_raw(),
						"f %i/%i/%i %i/%i/%i %i/%i/%i %i/%i/%i",
						face + 0, face + 1, face + 2,
						face + 3, face + 4, face + 5,
						face + 6, face + 7, face + 8,
						face + 9, face + 10, face + 11);

					faceWriter.write_elements<int>(face, 12);

					++faceCount;
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
			case 't': {
				bool containsW = (sscanf(liner.read_raw(), "vt %f %f %f",
					vertex + 0, vertex + 1, vertex + 2) == 3);

				if (!containsW)
					vertex[2] = 0;

				uvWriter.write_elements<float>(vertex, 3);
			}
					  break;
			case 'n':
				sscanf(liner.read_raw(), "vn %f %f %f",
					vertex + 0, vertex + 1, vertex + 2);
				normWriter.write_elements<float>(vertex, 3);

				++normalCount;
				break;
			case ' ':
				containsW = (sscanf(liner.read_raw(), "v %f %f %f %f",
					vertex + 0, vertex + 1, vertex + 2, vertex + 3) == 4);

				vertWriter.write_elements<float>(vertex, (containsW) ? 4 : 3);
				++vertexCount;
				break;
			}
		}
	}
	// populate the size of the last object
	temp = &object.at(object.size() - 1);
	temp->size = faceCount - temp->start;


	// Pack verticies

	CBuffer packed(4000);
	BufferWriter writer(&packed);
	// okay because indicies will read ahead of the writer
	BufferWriter newIndicies(&faces);
	BufferReader indicies(&faces);
	for (unsigned x = 0; x < faces.size(); ++x) {
		switch (face_format) {
		case FACE_FORMAT::VERTEX:
			packed.init(verticies.data(), verticies.size(), false);
			x = faces.size();
			break;
		case FACE_FORMAT::VERTEX_NORMAL:
			writer.write_elements<float>(verticies.data<float>() + (indicies.read<int>() * (containsW) ? 4 : 3), (containsW) ? 4 : 3);
			writer.write_elements<float>(normals.data<float>() + indicies.read<int>() * 3, sizeof(GLfloat) * 3);
			newIndicies.write<unsigned>(x);
			break;
		case FACE_FORMAT::VERTEX_UV:
			writer.write_elements<float>(verticies.data<float>() + (indicies.read<int>() * (containsW) ? 4 : 3), (containsW) ? 4 : 3);
			writer.write_elements<float>(uv.data<float>() + indicies.read<int>() * 3, sizeof(GLfloat) * 3);
			newIndicies.write<unsigned>(x);
			break;
		case FACE_FORMAT::VERTEX_UV_NORMAL:
			writer.write_elements<float>(verticies.data<float>() + (indicies.read<int>() * (containsW) ? 4 : 3), (containsW) ? 4 : 3);
			writer.write_elements<float>(uv.data<float>() + indicies.read<int>() * 3, sizeof(GLfloat) * 3);
			writer.write_elements<float>(normals.data<float>() + indicies.read<int>() * 3, sizeof(GLfloat) * 3);
			newIndicies.write<unsigned>(x);
			break;
		}
	}


	// Create VBO in OpenGL

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	auto value = vertexCount * sizeof(float) * ((containsW) ? 4 : 3); 
	glBufferData(GL_ARRAY_BUFFER, value, packed.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (glGetError() != GL_NO_ERROR) {
		error.reportError("Unable to load OBJ Model: Create Array Buffer");
		return nullptr;
	}


	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	value = faceCount * sizeof(GLuint) * ((face_type == GL_QUADS) ? 4 : 3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, value, faces.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (glGetError() != GL_NO_ERROR) {
		error.reportError("Unable to load OBJ Model: Create Element Array");
		return nullptr;
	}
	Mesh* m = new Mesh(vbo, ebo, face_type, containsW, !uv.isEmpty(), !normals.isEmpty());
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

	if (model == nullptr) {
		error.reportError("Unable to load model: Load fail\0");
		return nullptr;
	}

	return model;
}
Mesh* MeshLoader::loadModel(CBuffer&& buffer) {
	return loadModel(&buffer);
}