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

	float face[12];
	float vertex[16];

	MeshObject obj;
	MeshObject* temp = nullptr;

	while (reader.hasNext()) {
		auto line = reader.read_delim('\n', false);
		BufferReader liner(&line);

		// Check that the next line is creating an object if one hasn't been
		// skip the line if one isn't created  


		switch (line.data<char>()[0]) {
		case 'o':
			liner.skip(2);
			// pointer will break outside of this function
			object.push_back(MeshObject());

			obj.name = string(liner.read_delim('\n', false).data<char>());
			obj.start = faceCount;

			glGenVertexArrays(1, &obj.vba);
			glBindVertexArray(0);

			if (object.size() != 0) {
				temp = &object.at(object.size() - 2);
				temp->size = faceCount - temp->start;
			}
			break;

		case 'f':
			// TEST
			// Check for what kind of face

			liner.skip(2);

			if (line.count<char>(' ') == 3)
				face_type = GL_TRIANGLES;
			else
				face_type = GL_QUADS;

			switch (face_type)
			{
			case GL_TRIANGLES:
				if (liner.contains<char>("//", 2) != -1) {
					// Vector//Normal  
					sscanf(liner.read_raw(),
						"f %i//%i %i//%i %i//%i",
						face + 0, face + 1,
						face + 2, face + 3,
						face + 4, face + 5);

					faceWriter.write_elements<float>(face, 6);

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

					faceWriter.write_elements<float>(face, 3);

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

					faceWriter.write_elements<float>(face, 6);


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

					faceWriter.write_elements<float>(face, 9);

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
					faceWriter.write_elements<float>(face, 8);

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

					faceWriter.write_elements<float>(face, 4);

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

					faceWriter.write_elements<float>(face, 8);

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

					faceWriter.write_elements<float>(face, 12);

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
				bool containsW = (sscanf(liner.read_raw(), "vt %d %d %d",
					vertex + 0, vertex + 1, vertex + 2) == 3);

				if (!containsW)
					vertex[2] = 0;

				uvWriter.write_elements<float>(vertex, 3);
			}
				break;
			case 'n':
				sscanf(liner.read_raw(), "vn %d %d %d",
					vertex + 0, vertex + 1, vertex + 2);
				normWriter.write_elements<float>(vertex, 3);

				++normalCount;
				break;
			case ' ':
				containsW = (sscanf(liner.read_raw(), "v %d %d %d %d",
					vertex + 0, vertex + 1, vertex + 2, vertex + 3) == 4);

				vertWriter.write_elements<float>(vertex, (containsW) ? 3 : 4);
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
	for (int x = 0; x < faces.size(); ++x) {
		switch (face_format) {
		case FACE_FORMAT::VERTEX:
			packed.init(verticies.data(), verticies.size(), false);
			break;
		case FACE_FORMAT::VERTEX_NORMAL:
			writer.write_elements<float>(verticies.data<float>() + (indicies.read<int>() * (containsW) ? 3 : 4), (containsW) ? 3 : 4);
			writer.write_elements<float>(normals.data<float>() + indicies.read<int>() * 3, sizeof(GLfloat) * 3);
			newIndicies.write<int>(x);
			break;
		case FACE_FORMAT::VERTEX_UV:
			writer.write_elements<float>(verticies.data<float>() + (indicies.read<int>() * (containsW) ? 3 : 4), (containsW) ? 3 : 4);
			writer.write_elements<float>(uv.data<float>() + indicies.read<int>() * 3, sizeof(GLfloat) * 3);
			newIndicies.write<int>(x);
			break;
		case FACE_FORMAT::VERTEX_UV_NORMAL:
			writer.write_elements<float>(verticies.data<float>() + (indicies.read<int>() * (containsW) ? 3 : 4), (containsW) ? 3 : 4);
			writer.write_elements<float>(uv.data<float>() + indicies.read<int>() * 3, sizeof(GLfloat) * 3);
			writer.write_elements<float>(normals.data<float>() + indicies.read<int>() * 3, sizeof(GLfloat) * 3);
			newIndicies.write<int>(x);
			break;
		}
	}


	// Create VBO in OpenGL

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, packed.size(), packed.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceCount * sizeof(GLuint) * 3, faces.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Mesh* m = new Mesh(vbo, ebo, containsW, uv.isEmpty(), normals.isEmpty());
	for (int x = 0; x < object.size(); ++x)
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