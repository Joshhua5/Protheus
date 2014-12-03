#include "ModelLoader.h"

using namespace Pro;
using namespace Util;

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
			liner.skip(1); 
			currentObject->faceWriter->write<float>(atoi(liner.read_delim('\\', false).data<char>()));
			liner.skip(1);
			currentObject->faceWriter->write<float>(atoi(liner.read_delim(' ', false).data<char>()));
			 
			liner.skip(1);
			currentObject->faceWriter->write<float>(atoi(liner.read_delim('\\', false).data<char>()));
			liner.skip(1);
			currentObject->faceWriter->write<float>(atoi(liner.read_delim(' ', false).data<char>()));
			 
			liner.skip(1);
			currentObject->faceWriter->write<float>(atoi(liner.read_delim('\\', false).data<char>()));
			liner.skip(1);
			currentObject->faceWriter->write<float>(atoi(liner.read_delim(' ', false).data<char>()));
			 
			liner.skip(1);
			currentObject->faceWriter->write<float>(atoi(liner.read_delim('\\', false).data<char>()));
			liner.skip(1);
			currentObject->faceWriter->write<float>(atoi(liner.read_delim('\n', false).data<char>()));
			++currentObject->faceCount;
			break;

		case 'v':
			switch (line.data<char>()[1]) {
			case 'n':
				liner.skip(3);
				currentObject->normWriter->write<float>(atof(liner.read_delim(' ', false).data<char>()));
				currentObject->normWriter->write<float>(atof(liner.read_delim(' ', false).data<char>()));
				currentObject->normWriter->write<float>(atof(liner.read_delim('\n', false).data<char>()));
				++currentObject->normalCount;
				break;
			case ' ':
				liner.skip(2);
				currentObject->vertWriter->write<float>(atof(liner.read_delim(' ', false).data<char>()));
				currentObject->vertWriter->write<float>(atof(liner.read_delim(' ', false).data<char>()));
				currentObject->vertWriter->write<float>(atof(liner.read_delim('\n', false).data<char>()));
				++currentObject->vertexCount;
				break;
			}
		}

	}
	// Create VBO in OpenGL

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