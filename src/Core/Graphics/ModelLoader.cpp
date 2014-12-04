#include "ModelLoader.h"

using namespace Pro;
using namespace Util;

enum struct FACE_FORMAT {
	VERTEX,
	VERTEX_NORMAL,
	VERTEX_UV_NORMAL,
	VERTEX_UV
};

struct Object {
	// stores a pointer to the name inside of the loaded buffer
	char* name;
	CBuffer* verticies;
	BufferWriter* vertWriter;
	CBuffer* normals;
	BufferWriter* normWriter;
	CBuffer* faces;
	BufferWriter* faceWriter;
	unsigned vertexCount = 0;
	unsigned normalCount = 0;
	unsigned faceCount = 0;

	FACE_FORMAT face_format;
	GLenum face_type;

	Object() {
		verticies = new CBuffer(1);
		faces = new CBuffer(1);
		normals = new CBuffer(1);
		normWriter = new BufferWriter(normals);
		vertWriter = new BufferWriter(verticies);
		faceWriter = new BufferWriter(faces);
	}

	~Object() {
		delete vertWriter;
		delete faceWriter;
		delete normWriter;
		delete normals;
		delete verticies;
		delete faces;
	}

	void compact() {
		verticies->resize(vertexCount * sizeof(float) * 3);
		normals->resize(normalCount * sizeof(float) * 3);
		faces->resize(faceCount * sizeof(int) * 8);
	}
};

MODEL_FORMAT  ModelLoader::queryFormat(CBuffer* buffer) {

	if (buffer->isEmpty())
		return MODEL_FORMAT::UNDEFINED;

	//switch () {


	//default:
	//	return MODEL_FORMAT::UNDEFINED;
	//}
	return MODEL_FORMAT::UNDEFINED;
}

Model* ModelLoader::loadOBJ(CBuffer* buffer) {
	BufferReader reader(buffer);

	std::vector<Object> objects;

	Object* currentObject = nullptr;

	float face[12];
	float vertex[16];

	while (reader.hasNext()) {
		auto line = reader.read_delim('\n', false);
		BufferReader liner(&line);

		// Check that the next line is creating an object if one hasn't been
		// skip the line if one isn't created 
		if (currentObject == nullptr && line.data<char>()[0] != 'o')
			continue;

		switch (line.data<char>()[0]) {
		case 'o':
			objects.push_back(Object());
			currentObject = &objects.back();
			liner.skip(2);
			// pointer will break outside of this function
			currentObject->name = liner.read_delim('\n', false).data<char>();
			break;

		case 'f':
			// TEST
			// Check for what kind of face

			liner.skip(2);

			if (line.count<char>(' ') == 3)
				currentObject->face_type = GL_TRIANGLES;
			else
				currentObject->face_type = GL_QUADS;

			switch (currentObject->face_type)
			{
			case GL_TRIANGLES:
				if (liner.contains<char>("//", 2) != -1) {
					// Vector//Normal  
					sscanf(liner.read_raw(),
						"f %i//%i %i//%i %i//%i",
						face + 0, face + 1,
						face + 2, face + 3,
						face + 4, face + 5);

					currentObject->faceWriter->write_elements<float>(face, 6);

					++currentObject->faceCount;
					currentObject->face_format = FACE_FORMAT::VERTEX_NORMAL;
					break;
				}
				switch (line.count<char>('/')) {
				case 0:
					// simple vertex 
					sscanf(liner.read_raw(),
						"%*s %i %i %i",
						face + 0, face + 1,
						face + 2);

					currentObject->faceWriter->write_elements<float>(face, 3);

					++currentObject->faceCount;
					currentObject->face_format = FACE_FORMAT::VERTEX;
					break;
				case 4:
					// vertex/uv  
					// TEST does atoi stop at the /

					sscanf(liner.read_raw(),
						"f %i/%i %i/%i %i/%i",
						face + 0, face + 1,
						face + 2, face + 3,
						face + 4, face + 5);

					currentObject->faceWriter->write_elements<float>(face, 6);


					++currentObject->faceCount;
					currentObject->face_format = FACE_FORMAT::VERTEX_UV;
					break;
				case 8:
					// vertex/uv/normal 
					// TEST does atoi stop at the /

					sscanf(liner.read_raw(),
						"f %i/%i/%i %i/%i/%i %i/%i/%i",
						face + 0, face + 1, face + 2,
						face + 3, face + 4, face + 5,
						face + 6, face + 7, face + 8);

					currentObject->faceWriter->write_elements<float>(face, 9);

					++currentObject->faceCount;
					currentObject->face_format = FACE_FORMAT::VERTEX_UV_NORMAL;
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
					currentObject->faceWriter->write_elements<float>(face, 8);

					++currentObject->faceCount;
					currentObject->face_format = FACE_FORMAT::VERTEX_NORMAL;
					break;
				}
				switch (line.count<char>('/')) {
				case 0:
					// simple vertex 
					sscanf(liner.read_raw(),
						"f %i %i %i %i",
						face + 0, face + 1,
						face + 2, face + 3);

					currentObject->faceWriter->write_elements<float>(face, 4);

					++currentObject->faceCount;
					currentObject->face_format = FACE_FORMAT::VERTEX;
					break;
				case 4:
					// vertex/uv  
					// TEST does atoi stop at the /
					sscanf(liner.read_raw(),
						"f %i/%i %i/%i %i/%i %i/%i",
						face + 0, face + 1, face + 2,
						face + 3, face + 4, face + 5,
						face + 6, face + 7);

					currentObject->faceWriter->write_elements<float>(face, 8);

					++currentObject->faceCount;
					currentObject->face_format = FACE_FORMAT::VERTEX_UV;
					break;
				case 8:
					// vertex/uv/normal 
					// TEST does atoi stop at the /
					sscanf(liner.read_raw(),
						"f %i//%i//%i %i//%i//%i %i//%i//%i %i//%i//%i",
						face + 0, face + 1, face + 2,
						face + 3, face + 4, face + 5,
						face + 6, face + 7, face + 8,
						face + 9, face + 10, face + 11);

					currentObject->faceWriter->write_elements<float>(face, 12);

					++currentObject->faceCount;
					currentObject->face_format = FACE_FORMAT::VERTEX_UV_NORMAL;
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
			case 'n':
				sscanf(liner.read_raw(),"vn %d %d %d",
					vertex + 0, vertex + 1, vertex + 2);
				currentObject->normWriter->write_elements<float>(vertex, 3);

				++currentObject->normalCount;
				break;
			case ' ':
				currentObject->vertWriter->write_elements<float>(
					vertex,
					sscanf(liner.read_raw(), "v %d %d %d %d",
					vertex + 0, vertex + 1, vertex + 2, vertex + 3));

				++currentObject->vertexCount;
				break;
			}
		}

	}
	// Create VBO in OpenGL

	for each(const auto& obj in objects) {


	}

	return nullptr;
}


Model* ModelLoader::loadModel(CBuffer* buffer) {
	Model* model = nullptr;

	if (buffer->isEmpty()) {
		error.reportError("Empty buffer passed to ModelLoader, did file load correctly?\0");
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
Model* ModelLoader::loadModel(CBuffer&& buffer) {
	return loadModel(&buffer);
}